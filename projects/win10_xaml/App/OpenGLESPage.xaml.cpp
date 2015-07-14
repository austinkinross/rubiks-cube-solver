#include "pch.h"
#include "OpenGLESPage.xaml.h"

using namespace RubiksCubeXAMLApp;
using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;

OpenGLESPage::OpenGLESPage() :
    mRenderSurface(EGL_NO_SURFACE)
{
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
    if (args->Visible && mRenderSurface != EGL_NO_SURFACE)
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

}

void OpenGLESPage::DestroyRenderSurface()
{

}

void OpenGLESPage::RecoverFromLostDevice()
{
    // Stop the render loop, reset OpenGLES, recreate the render surface
    // and start the render loop again to recover from a lost device.

    StopRenderLoop();

    {
        critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

        DestroyRenderSurface();
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

        while (action->Status == Windows::Foundation::AsyncStatus::Started)
        {

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