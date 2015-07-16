using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Foundation.Collections;
using Windows.Media.Capture;
using Windows.Media.Effects;
using Windows.UI.Xaml.Controls;

namespace RubiksCubeRecognitionWinRT
{
    public sealed class CSharpRecognizer
    {
        private MediaCapture mMediaCapture;

        public CSharpRecognizer(CaptureElement captureElement, TextBlock progressText)
        {
            mMediaCapture = new MediaCapture();

            var devices = DeviceInformation.FindAllAsync(DeviceClass.VideoCapture).AsTask();

            string deviceID;

            if (devices.Result.Count >= 2)
            {
                deviceID = devices.Result[1].Id;
            }
            else
            {
                deviceID = devices.Result[0].Id;
            }

            devices.Wait();

            var settings = new MediaCaptureInitializationSettings()
            {
                StreamingCaptureMode = StreamingCaptureMode.Video,
                VideoDeviceId = deviceID
            };

            try
            {
                mMediaCapture.InitializeAsync(settings).AsTask().Wait();
            }
            catch (Exception)
            {
                progressText.Text = "No camera is available.";
                return;
            }

            // This doesn't work on ASUST100TAM.
            // https://msdn.microsoft.com/en-us/library/windows.media.capture.mediacapture.setpreviewmirroring.aspx
            // Use 'FlowDirection' on the CaptureElement instead
            // mMediaCapture.SetPreviewMirroring(true);

            captureElement.Source = mMediaCapture;
            mMediaCapture.StartPreviewAsync().AsTask().Wait();

            mMediaCapture.AddVideoEffectAsync(
                new VideoEffectDefinition(typeof(AnalysisEffect).FullName, new PropertySet()),
                MediaStreamType.VideoPreview).AsTask().Wait();
        }

        public bool getBool()
        {
            return false;
        }
    }
}
