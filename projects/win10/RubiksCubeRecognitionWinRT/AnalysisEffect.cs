using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.Effects;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Numerics;
using System.Threading;
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

        static Color[] cubletColors = new Color[3 * 3];
        static Mutex cubeletColorsMutex = new Mutex();

        public static Color[] GetLastCubletColors()
        {
          //  cubeletColorsMutex.WaitOne();

            Color[] colors = new Color[9];
            cubletColors.CopyTo(colors, 0);

          //  cubeletColorsMutex.ReleaseMutex();

            return colors;
        }

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

            int centerX = 0;
            int centerY = 0;
            int cubletWidth = 0;
            int cubletHeight = 0;
                
            int analysisAreaX = (int)rt.SizeInPixels.Width * 3 / 10;
            int analysisWidth = (int)rt.SizeInPixels.Width * 4 / 10;
            byte[] analysisHorzBytes = rt.GetPixelBytes(analysisAreaX, (int)rt.SizeInPixels.Height / 2, analysisWidth, 1);

            int analysisAreaY = (int)rt.SizeInPixels.Height * 3 / 10;
            int analysisHeight = (int)rt.SizeInPixels.Height * 4 / 10;
            byte[] analysisVertBytes = rt.GetPixelBytes((int)rt.SizeInPixels.Width / 2, analysisAreaY, 1, analysisHeight);

            int foundLeft = 0;
            for (int i = 0; i < analysisWidth / 2; i++)
            {
                byte b = analysisHorzBytes[4 * (analysisWidth / 2 - i) + 0];
                byte g = analysisHorzBytes[4 * (analysisWidth / 2 - i) + 1];
                byte r = analysisHorzBytes[4 * (analysisWidth / 2 - i) + 2];
                if ((r > 100 ||  g > 100 || b > 50) && foundLeft == 0)
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
                if ((r > 100 || g > 100 || b > 50) && foundRight == 0)
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
                if ((r > 100 || g > 100 || b > 50) && foundTop == 0)
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
                if ((r > 100 || g > 100 || b > 50) && foundBottom == 0)
                {
                    foundBottom = i;
                }
            }

            centerX = (-foundLeft + foundRight) / 2 + (int)rt.SizeInPixels.Width / 2;
            centerY = (-foundTop + foundBottom) / 2 + (int)rt.SizeInPixels.Height / 2;
            cubletWidth = (int)((foundLeft + foundRight) * 1.2f);
            cubletHeight = (int)((foundTop + foundBottom) * 1.2f);

            // No 2d arrays in WinRT components? Boo.
            // "Error C1113	#using failed on 'rubikscuberecognitionwinrt.winmd'	RubiksCubeRecognitionLib
            Vector2[] cubletCenters = new Vector2[3 * 3];

            cubletCenters[1 * 3 + 1] = new Vector2(centerX, centerY);
            cubletCenters[1 * 3 + 0] = new Vector2(centerX, centerY - cubletHeight);
            cubletCenters[1 * 3 + 2] = new Vector2(centerX, centerY + cubletHeight);
            cubletCenters[0 * 3 + 1] = new Vector2(centerX - cubletWidth, centerY);
            cubletCenters[0 * 3 + 0] = new Vector2(centerX - cubletWidth, centerY - cubletHeight);
            cubletCenters[0 * 3 + 2] = new Vector2(centerX - cubletWidth, centerY + cubletHeight);
            cubletCenters[2 * 3 + 1] = new Vector2(centerX + cubletWidth, centerY);
            cubletCenters[2 * 3 + 0] = new Vector2(centerX + cubletWidth, centerY - cubletHeight);
            cubletCenters[2 * 3 + 2] = new Vector2(centerX + cubletWidth, centerY + cubletHeight);

            cubeletColorsMutex.WaitOne();

            using (CanvasBitmap input = CanvasBitmap.CreateFromDirect3D11Surface(canvasDevice, context.InputFrame.Direct3DSurface))
            using (CanvasRenderTarget output = CanvasRenderTarget.CreateFromDirect3D11Surface(canvasDevice, context.OutputFrame.Direct3DSurface))
            using (CanvasDrawingSession ds = output.CreateDrawingSession())
            {
                var transformEffect = new Transform2DEffect()
                {
                    Source = input,
                    TransformMatrix = new System.Numerics.Matrix3x2(-1, 0, 0, 1, output.SizeInPixels.Width, 0),
                };

                Rect src = new Rect(0, 0, output.SizeInPixels.Width, output.SizeInPixels.Height);
                ds.DrawImage(transformEffect, src, src);

                // Draw a crosshair on the screen
                ds.FillRectangle(new Rect(output.SizeInPixels.Width / 2 - 3, output.SizeInPixels.Height / 4, 6, output.SizeInPixels.Height / 2), Colors.Gray);
                ds.FillRectangle(new Rect(output.SizeInPixels.Width / 4, output.SizeInPixels.Height / 2 - 3, output.SizeInPixels.Width / 2, 6), Colors.Gray);

                if (true)
                {
                    for (int x = 0; x < 3; x++)
                    {
                        for (int y = 0; y < 3; y++)
                        {
                            int sampleWidth = 2;

                            byte[] cubletBytes = input.GetPixelBytes((int)cubletCenters[(2 - x) * 3 + y].X - sampleWidth / 2, (int)cubletCenters[(2 - x) * 3 + y].Y - sampleWidth / 2, sampleWidth, sampleWidth);
                            int totalR = 0; int totalG = 0; int totalB = 0;
                            for (int i = 0; i < sampleWidth * sampleWidth; i++)
                            {
                                totalB += cubletBytes[4 * i + 0];
                                totalG += cubletBytes[4 * i + 1];
                                totalR += cubletBytes[4 * i + 2];
                            }

                            cubletColors[x * 3 + y] = Color.FromArgb(255, (byte)(totalR / (sampleWidth * sampleWidth)), (byte)(totalG / (sampleWidth * sampleWidth)), (byte)(totalB / (sampleWidth * sampleWidth)));
                        }
                    }

                    for (int x = 0; x < 3; x++)
                    {
                        for (int y = 0; y < 3; y++)
                        {
                            ds.FillRectangle((float)cubletCenters[x * 3 + y].X - 12, (float)cubletCenters[x * 3 + y].Y - 12, 24, 24, Colors.Black);
                            ds.FillRectangle((float)cubletCenters[x * 3 + y].X - 10, (float)cubletCenters[x * 3 + y].Y - 10, 20, 20, cubletColors[x * 3 + y]);
                        }
                    }
                }
            }

            cubeletColorsMutex.ReleaseMutex();
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