#include "pch.h"
#include "OpenGLESPage.xaml.h"

#include "WindowsStore/BasicTimer.h"

using namespace RubiksCubeXAMLApp;
using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;

OpenGLESPage::OpenGLESPage() :
	OpenGLESPage(nullptr)
{
}

OpenGLESPage::OpenGLESPage(OpenGLES* openGLES) :
	mOpenGLES(openGLES),
	mRenderSurface(EGL_NO_SURFACE),
	mCreatedRenderSurface(false)
{
	mAgnosticApp = new AgnosticApp();

	InitializeComponent();

	Windows::UI::Core::CoreWindow^ window = Windows::UI::Xaml::Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &OpenGLESPage::OnVisibilityChanged);

	this->Loaded +=
		ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLESPage::OnPageLoaded);
}

OpenGLESPage::~OpenGLESPage()
{
	StopRenderLoop();
	DestroyRenderSurface();
}

void OpenGLESPage::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// The SwapChainPanel has been created and arranged in the page layout, so EGL can be initialized.
	CreateRenderSurface();
	StartRenderLoop();
}

void OpenGLESPage::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	if (args->Visible && mCreatedRenderSurface)
	{
		StartRenderLoop();
	}
	else
	{
		StopRenderLoop();
	}
}

void OpenGLESPage::CreateRenderSurface()
{
	if (mOpenGLES && !mCreatedRenderSurface)
	{
		WindowWrapper windowWrapper;
		windowWrapper.swapChainPanel = reinterpret_cast<void*>(swapChainPanel);

		// mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, nullptr);
		mAgnosticApp->Initialize(&windowWrapper, swapChainPanel->ActualWidth, swapChainPanel->ActualHeight);

		mCreatedRenderSurface = true;
	}
}

void OpenGLESPage::DestroyRenderSurface()
{
	if (mOpenGLES)
	{
		mOpenGLES->DestroySurface(mRenderSurface);
	}
	mRenderSurface = EGL_NO_SURFACE;
	mCreatedRenderSurface = false;
}

void OpenGLESPage::RecoverFromLostDevice()
{
	// Stop the render loop, reset OpenGLES, recreate the render surface
	// and start the render loop again to recover from a lost device.

	StopRenderLoop();

	{
		critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

		DestroyRenderSurface();
		mOpenGLES->Reset();
		CreateRenderSurface();
	}

	StartRenderLoop();
}

void OpenGLESPage::StartRenderLoop()
{
	// If the render loop is already running then do not start another thread.
	if (mRenderLoopWorker != nullptr && mRenderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
	{
		return;
	}

	// Create a task for rendering that will be run on a background thread.
	auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction ^ action)
	{
		critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

		BasicTimer^ timer = ref new BasicTimer();

		//mOpenGLES->MakeCurrent(mRenderSurface);
		mAgnosticApp->MakeCurrent();

		while (action->Status == Windows::Foundation::AsyncStatus::Started)
		{
			// glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			timer->Update();
			mAgnosticApp->Update(timer->Total, timer->Delta);
			mAgnosticApp->Render();

			// mOpenGLES->SwapBuffers(mRenderSurface);
			mAgnosticApp->Present();

			//////// The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
			//////// If the call fails, then we must reinitialize EGL and the GL resources.
			//if (mOpenGLES->SwapBuffers(mRenderSurface) != GL_TRUE)
			//{
			//	// XAML objects like the SwapChainPanel must only be manipulated on the UI thread.
			//	swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
			//	{
			//		RecoverFromLostDevice();
			//	}, CallbackContext::Any));

			//	return;
			//}
		}
	});

	// Run task on a dedicated high priority background thread.
	mRenderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::High, Windows::System::Threading::WorkItemOptions::TimeSliced);
}

void OpenGLESPage::StopRenderLoop()
{
	if (mRenderLoopWorker)
	{
		mRenderLoopWorker->Cancel();
		mRenderLoopWorker = nullptr;
	}
}