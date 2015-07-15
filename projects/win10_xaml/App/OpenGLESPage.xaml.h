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

	private:
		void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void CreateAgnosticApp();
		void DestroyAgnosticApp();
		void StartRenderLoop();
		void StopRenderLoop();

		AgnosticApp *mAgnosticApp;

		Windows::Foundation::IAsyncAction^ mRenderLoopWorker;
	};
}
