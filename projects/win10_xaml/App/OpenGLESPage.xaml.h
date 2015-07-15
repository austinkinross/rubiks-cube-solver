#pragma once

#include "OpenGLES.h"
#include "App/OpenGLESPage.g.h"
#include "AgnosticApp.h"

namespace RubiksCubeXAMLApp
{
	public ref class OpenGLESPage sealed
	{
	public:
		OpenGLESPage();
		virtual ~OpenGLESPage();

	internal:
		OpenGLESPage(OpenGLES* openGLES);

	private:
		void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void CreateRenderSurface();
		void DestroyRenderSurface();
		void RecoverFromLostDevice();
		void StartRenderLoop();
		void StopRenderLoop();

		AgnosticApp *mAgnosticApp;
		OpenGLES* mOpenGLES;

		bool mCreatedRenderSurface;
		EGLSurface mRenderSurface;     // This surface is associated with a swapChainPanel on the page
		Concurrency::critical_section mRenderSurfaceCriticalSection;
		Windows::Foundation::IAsyncAction^ mRenderLoopWorker;
	};
}
