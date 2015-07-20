#include "pch.h"
#include "OpenGLESPage.xaml.h"

#include "WindowsStore/BasicTimer.h"

using namespace RubiksCubeXAMLApp;
using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;

OpenGLESPage::OpenGLESPage() :
	mAgnosticApp(nullptr)
{
	InitializeComponent();

	Windows::UI::Core::CoreWindow^ window = Windows::UI::Xaml::Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &OpenGLESPage::OnVisibilityChanged);

	this->Loaded +=
		ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLESPage::OnPageLoaded);

	swapChainPanel->SizeChanged +=
		ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &OpenGLESPage::OnSwapChainPanelSizeChanged);

//	captureSolveButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLESPage::captureSolveButton_Click);
}

OpenGLESPage::~OpenGLESPage()
{
	StopRenderLoop();
	DestroyAgnosticApp();
}

void OpenGLESPage::OnSwapChainPanelSizeChanged(Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	// Size change events occur outside of the render thread.  A lock is required when updating  
	// the swapchainpanel size
	if (mAgnosticApp)
	{
		mAgnosticApp->UpdateForWindowSizeChange(e->NewSize.Width, e->NewSize.Height);
	}
}

void OpenGLESPage::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	CreateAgnosticApp();
	StartRenderLoop();
}

void OpenGLESPage::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	if (args->Visible && mAgnosticApp != nullptr)
	{
		StartRenderLoop();
	}
	else
	{
		StopRenderLoop();
	}
}

void OpenGLESPage::CreateAgnosticApp()
{
	if (mAgnosticApp == nullptr)
	{
		WindowWrapper windowWrapper;
		windowWrapper.swapChainPanel = reinterpret_cast<void*>(swapChainPanel);
		windowWrapper.cameraPreviewPane = reinterpret_cast<void*>(captureElement);
		windowWrapper.cameraErrorText = reinterpret_cast<void*>(progressText);

		mAgnosticApp = new AgnosticApp();
		mAgnosticApp->Initialize(&windowWrapper, swapChainPanel->ActualWidth, swapChainPanel->ActualHeight);
	}
}

void OpenGLESPage::DestroyAgnosticApp()
{
	if (mAgnosticApp != nullptr)
	{
		delete mAgnosticApp;
		mAgnosticApp = nullptr;
	}
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
		BasicTimer^ timer = ref new BasicTimer();

		mAgnosticApp->MakeCurrent();

		while (action->Status == Windows::Foundation::AsyncStatus::Started)
		{
			timer->Update();
			mAgnosticApp->Update(timer->Total, timer->Delta);

			mAgnosticApp->Render();

			mAgnosticApp->Present();
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

void OpenGLESPage::captureSolveButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	mAgnosticApp->HandleCaptureStartButtonPress();
}
