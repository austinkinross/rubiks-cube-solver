using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.Capture;
using Windows.UI.Xaml.Controls;

namespace RubiksCubeRecognitionWinRT
{
    public sealed class Class1
    {
        private MediaCapture mMediaCapture;

        public Class1(CaptureElement captureElement)
        {
            mMediaCapture = new MediaCapture();

            var settings = new MediaCaptureInitializationSettings()
            {
                StreamingCaptureMode = StreamingCaptureMode.Video
            };

            mMediaCapture.InitializeAsync(settings).AsTask().Wait();

            captureElement.Source = mMediaCapture;
            mMediaCapture.StartPreviewAsync().AsTask().Wait();
        }

        public bool getBool()
        {
            return false;
        }
    }
}
