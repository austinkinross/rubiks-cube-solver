using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.Effects;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Numerics;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.DirectX.Direct3D11;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;
using Windows.UI;

namespace RubiksCubeRecognitionWinRT
{
    public sealed class AnalysisEffect : IBasicVideoEffect
    {
        CanvasDevice canvasDevice;

        public bool IsReadOnly
        {
            get
            {
                return true;
            }
        }

        public IReadOnlyList<VideoEncodingProperties> SupportedEncodingProperties
        {
            get
            {
                return new List<VideoEncodingProperties>();
            }
        }

        public MediaMemoryTypes SupportedMemoryTypes
        {
            get
            {
                return MediaMemoryTypes.Gpu;
            }
        }

        public bool TimeIndependent
        {
            get
            {
                return false;
            }
        }

        public void Close(MediaEffectClosedReason reason)
        {
            if (canvasDevice != null) { canvasDevice.Dispose(); }
        }

        public void DiscardQueuedFrames()
        {

        }

        public void ProcessFrame(ProcessVideoFrameContext context)
        {
            CanvasRenderTarget rt = new CanvasRenderTarget(canvasDevice, context.OutputFrame.Direct3DSurface.Description.Width, context.OutputFrame.Direct3DSurface.Description.Height, 96.0f);

            using (CanvasBitmap input = CanvasBitmap.CreateFromDirect3D11Surface(canvasDevice, context.InputFrame.Direct3DSurface))
            using (CanvasDrawingSession ds = rt.CreateDrawingSession())
            {
                TimeSpan time = context.InputFrame.RelativeTime.HasValue ? context.InputFrame.RelativeTime.Value : new TimeSpan();

                float dispX = (float)Math.Cos(time.TotalSeconds) * 75f;
                float dispY = (float)Math.Sin(time.TotalSeconds) * 75f;

                ds.Clear(Colors.Black);

                //var posterizeEffect = new PosterizeEffect()
                //{
                //    Source = input,
                //    BlueValueCount = 2,
                //    RedValueCount = 2,
                //    GreenValueCount = 2,
                //};

                var transformEffect = new Transform2DEffect()
                {
                    Source = input,
                    TransformMatrix = new System.Numerics.Matrix3x2(-1, 0, 0, 1, rt.SizeInPixels.Width, 0),
                };

                var discreteTransferEffect = new DiscreteTransferEffect()
                {
                    Source = transformEffect,
                    RedTable = new float[] { 0.0f, 1.0f, 1.0f, 1.0f },
                    GreenTable = new float[] { 0.0f, 1.0f, 1.0f, 1.0f },
                    BlueTable = new float[] { 0.0f, 1.0f, 1.0f, 1.0f },
                    AlphaDisable = true,
                };

                var dispMap = new ConvolveMatrixEffect()
                {
                    KernelMatrix = new float[]
                    {
                       0, +1,  0,
                      +1, -4, +1,
                       0, +1,  0,
                    },
                    Source = discreteTransferEffect,
                    Divisor = 0.2f,
                };

                var modEffect = new ArithmeticCompositeEffect()
                {
                    Source1 = dispMap,
                    Source2 = dispMap,
                    MultiplyAmount = 1,
                };

                //var finalPosterizeEffect = new PosterizeEffect()
                //{
                //    Source = modEffect,
                //    BlueValueCount = 2,
                //    RedValueCount = 2,
                //    GreenValueCount = 2,
                //};

                //var dispMap = new EdgeDetectionEffect()
                //{
                //    Source = greyScaleEffect,
                //    Mode = EdgeDetectionEffectMode.Sobel,
                //};

                Rect src = new Rect(rt.SizeInPixels.Width / 3, rt.SizeInPixels.Height / 3, rt.SizeInPixels.Width / 3, rt.SizeInPixels.Height / 3);

                //ds.DrawImage(input, bottomLeft);
                //ds.DrawImage(discreteTransferEffect, bottomRight, src);
                //ds.DrawImage(dispMap, topLeft, src);
                ds.DrawImage(modEffect, src, src);
            }

            using (CanvasBitmap input = CanvasBitmap.CreateFromDirect3D11Surface(canvasDevice, context.InputFrame.Direct3DSurface))
            using (CanvasRenderTarget output = CanvasRenderTarget.CreateFromDirect3D11Surface(canvasDevice, context.OutputFrame.Direct3DSurface))
            using (CanvasDrawingSession ds = output.CreateDrawingSession())
            {
                Debug.WriteLine(output.Format.ToString());
                
                int analysisAreaX = (int)rt.SizeInPixels.Width * 3 / 10;
                int analysisWidth = (int)rt.SizeInPixels.Width * 4 / 10;
                byte[] analysisHorzBytes = rt.GetPixelBytes(analysisAreaX, (int)output.SizeInPixels.Height / 2, analysisWidth, 1);

                int analysisAreaY = (int)rt.SizeInPixels.Height * 3 / 10;
                int analysisHeight = (int)rt.SizeInPixels.Height * 4 / 10;
                byte[] analysisVertBytes = rt.GetPixelBytes((int)output.SizeInPixels.Width / 2, analysisAreaY, 1, analysisHeight);

                int foundLeft = 0;
                for (int i = 0; i < analysisWidth / 2; i++)
                {
                    byte r = analysisHorzBytes[4 * (analysisWidth / 2 - i) + 0];
                    byte g = analysisHorzBytes[4 * (analysisWidth / 2 - i) + 1];
                    byte b = analysisHorzBytes[4 * (analysisWidth / 2 - i) + 2];
                    if ((r > 100 ||  g > 100 || b > 100) && foundLeft == 0)
                    {
                        foundLeft = i;
                    }
                }

                int foundRight = 0;
                for (int i = 0; i < analysisWidth / 2; i++)
                {
                    byte r = analysisHorzBytes[4 * (analysisWidth / 2 + i) + 0];
                    byte g = analysisHorzBytes[4 * (analysisWidth / 2 + i) + 1];
                    byte b = analysisHorzBytes[4 * (analysisWidth / 2 + i) + 2];
                    if ((r > 100 || g > 100 || b > 100) && foundRight == 0)
                    {
                        foundRight = i;
                    }
                }

                int foundTop = 0;
                for (int i = 0; i < analysisHeight / 2; i++)
                {
                    byte r = analysisVertBytes[4 * (analysisHeight / 2 - i) + 0];
                    byte g = analysisVertBytes[4 * (analysisHeight / 2 - i) + 1];
                    byte b = analysisVertBytes[4 * (analysisHeight / 2 - i) + 2];
                    if ((r > 100 || g > 100 || b > 100) && foundTop == 0)
                    {
                        foundTop = i;
                    }
                }

                int foundBottom = 0;
                for (int i = 0; i < analysisHeight / 2; i++)
                {
                    byte r = analysisVertBytes[4 * (analysisHeight / 2 + i) + 0];
                    byte g = analysisVertBytes[4 * (analysisHeight / 2 + i) + 1];
                    byte b = analysisVertBytes[4 * (analysisHeight / 2 + i) + 2];
                    if ((r > 100 || g > 100 || b > 100) && foundBottom == 0)
                    {
                        foundBottom = i;
                    }
                }

                int centerX = (-foundLeft + foundRight) / 2 + (int)output.SizeInPixels.Width / 2;
                int centerY = (-foundTop + foundBottom) / 2 + (int)output.SizeInPixels.Height / 2;
                int cubletWidth = foundLeft + foundRight;
                int cubletHeight = foundTop + foundBottom;


                var transformEffect = new Transform2DEffect()
                {
                    Source = input,
                    TransformMatrix = new System.Numerics.Matrix3x2(-1, 0, 0, 1, output.SizeInPixels.Width, 0),
                };

                Rect src = new Rect(0, 0, output.SizeInPixels.Width, output.SizeInPixels.Height);
                ds.DrawImage(transformEffect, src, src);

                ds.FillRectangle(new Rect(output.SizeInPixels.Width / 2 - 3, output.SizeInPixels.Height / 4, 6, output.SizeInPixels.Height / 2), Colors.Gray);
                ds.FillRectangle(new Rect(output.SizeInPixels.Width / 4, output.SizeInPixels.Height / 2 - 3, output.SizeInPixels.Width / 2, 6), Colors.Gray);

                ds.FillRectangle(centerX - 10, centerY - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX - 10, centerY - cubletHeight - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX - 10, centerY + cubletHeight - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX - cubletWidth - 10, centerY - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX - cubletWidth - 10, centerY - cubletHeight - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX - cubletWidth - 10, centerY + cubletHeight - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX + cubletWidth - 10, centerY - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX + cubletWidth - 10, centerY - cubletHeight - 10, 20, 20, Colors.Red);
                ds.FillRectangle(centerX + cubletWidth - 10, centerY + cubletHeight - 10, 20, 20, Colors.Red);
            }

        }

        public void SetEncodingProperties(VideoEncodingProperties encodingProperties, IDirect3DDevice device)
        {
            canvasDevice = CanvasDevice.CreateFromDirect3D11Device(device);
        }

        public void SetProperties(IPropertySet configuration)
        {

        }
    }
}