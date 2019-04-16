using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;
using Windows.Storage;

namespace AudioVisualizer.test
{
    [TestClass]
    public class AudioViewDataCtorTests
    {
        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_ctor_LT4DownsampleThrows()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new AudioViewData(1024, 1, 0);
                }
                );
        }
        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_ctor_Non4MultipleDownsampleThrows()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new AudioViewData(1024, 1, 5);
                }
                );
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_ctor_ZeroDetailLevelThrows()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new AudioViewData(1024, 0, 44);
                }
                );
        }
        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_ctor_DetailLevelGT15Throws()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new AudioViewData(1024, 16, 44);
                }
                );
        }
    }

    [TestClass]
    public class AudioViewDataTests
    {
        AudioViewData sut;

        readonly uint expectedLevels = 3;

        [TestInitialize]
        public void Init()
        {
            sut = new AudioViewData(4096, expectedLevels, 4);
        }
        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_IsIVectorView()
        {
            Assert.IsInstanceOfType(sut, typeof(IReadOnlyList<IReadOnlyList<float>>));
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_IsIIterable()
        {
            Assert.IsInstanceOfType(sut, typeof(IEnumerable<IReadOnlyList<float>>));
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_LevelCountMethod()
        {
            Assert.AreEqual(expectedLevels, (uint)sut.Count());
        }
        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_LevelCountProperty()
        {
            Assert.AreEqual(expectedLevels, (uint)sut.Count);
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_add()
        {
            var values = new float[] { 1f, 2f, 3f, 4f, 5f, 6f, 7f, 8f, 9f, 10f, 11f, 12f, 13f, 14f, 15f, 16f };
            sut.Add(values.AsEnumerable());
        }
    }
        /*
        [TestClass]
        public class AudioViewDataBufferTests
        {
            AudioViewDataBuffer sut;
            [TestInitialize]
            public void Init()
            {
                sut = new AudioViewDataBuffer(1024, 48, 3);
            }


            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_AfterInit_DataIsNotNull()
            {
                Assert.IsNotNull(sut.Data);
            }
            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_AfterInit_DataHasThreeLevels()
            {
                Assert.AreEqual(3, sut.Data.Count);
            }
            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_AfterInit_DataLevelsAreNotNull()
            {
                CollectionAssert.AllItemsAreNotNull(sut.Data.ToArray());
            }

            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_AfterInit_DataLevelsAreAllEmpty()
            {
                foreach (var item in sut.Data)
                {
                    Assert.IsFalse(item.Any());
                }
            }


            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_Append_ZeroStrideThrows()
            {
                Assert.ThrowsException<ArgumentException>(() =>
                {
                    sut.Append(new Windows.Media.AudioFrame(480), 0, 0);
                });
            }

            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_Append_OffsetGTStrideThrows()
            {
                Assert.ThrowsException<ArgumentException>(() =>
                {
                    sut.Append(new Windows.Media.AudioFrame(480), 2, 2);
                });
            }

            [DataTestMethod]
            [TestCategory("AudioViewDataBuffer")]
            [DataRow(44, new int[] { 0, 0, 0 })]
            [DataRow(48, new int[] { 1, 0, 0 })]
            [DataRow(48 * 3 + 44, new int[] { 3, 0, 0 })]
            [DataRow(192, new int[] { 4, 1, 0 })]
            [DataRow(480, new int[] { 10, 2, 0 })]
            [DataRow(16 * 48 - 4, new int[] { 15, 3, 0 })]
            [DataRow(16 * 48, new int[] { 16, 4, 1 })]
            public void AudioViewDataBuffer_Append_LevelCountAreCorrect(int sampleCount, int[] expectedCounts)
            {
                var frame = new Windows.Media.AudioFrame((uint)(sampleCount * sizeof(float)));
                sut.Append(frame);
                var levelCounts = sut.Data.Select((data) => data.Count).ToArray();
                CollectionAssert.AreEqual(expectedCounts, levelCounts);
            }

            double SumOfSquares(int n)
            {
                return ((double)n) * ((double)(n + 1)) * ((double)(2 * n + 1)) / 6;
            }
            double SumOfSquares(int from, int to)
            {
                return SumOfSquares(to) - SumOfSquares(from != 0 ? from - 1 : 0);
            }

            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_Append_Level0_ValuesAreCorrect()
            {
                var frame = new Windows.Media.AudioFrame(1920);
                frame.Generate(1, 0, (sampleIndex, channelIndex) => { return (float)sampleIndex; });
                sut.Append(frame);
                for (int i = 0; i < 10; i++)
                {
                    float expectedValue = (float)(Math.Sqrt(SumOfSquares(i * 48, i * 48 + 47)) / 48.0);
                    Assert.AreEqual(expectedValue, sut.Data[0][i], 1e-5);
                }
            }

            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_Append_Level1_ValuesAreCorrect()
            {
                var frame = new Windows.Media.AudioFrame(48 * 16 * 4);  // This will give 4 items in level 1
                frame.Generate(1, 0, (sampleIndex, channelIndex) => { return (float)sampleIndex; });
                sut.Append(frame);
                for (int i = 0; i < 4; i++)
                {
                    float expectedValue = (float)(Math.Sqrt(SumOfSquares(i * 192, i * 192 + 191)) / 192.0);
                    Assert.AreEqual(expectedValue, sut.Data[1][i], 1e-5);
                }
            }

            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public void AudioViewDataBuffer_Performance()
            {
                var frame = new Windows.Media.AudioFrame(6400000);
                frame.Generate(1, 0, (sampleIndex, channelIndex) => { return (float)sampleIndex; });
                var sw = System.Diagnostics.Stopwatch.StartNew();
                sut.Append(frame);
                sw.Stop();
                Logger.LogMessage($"30sec data took {sw.Elapsed}");
                Assert.IsTrue(sw.ElapsedMilliseconds < 1000, $"Adding 30s data took {sw.Elapsed}");
            }
        }

        [TestClass]
        [TestCategory("AudioViewDataBuffer")]
        public class AudioViewDataBufferCtorTests
        {
            [DataTestMethod()]
            [DataRow(0)]
            [DataRow(1)]
            [DataRow(2)]
            [DataRow(3)]
            [DataRow(5)]
            public void AudioViewDataBuffer_CtorThrowsOnInvalidDownSampleRate(int downSampleRate)
            {
                Assert.ThrowsException<ArgumentException>(() =>
                {
                    var data = new AudioViewDataBuffer(0, (UInt32)downSampleRate, 1);
                });
            }
            [DataTestMethod()]
            [DataRow(0)]
            [DataRow(16)]
            public void AudioViewDataBuffer_CtorThrowsOnInvalidDetailLevel(int detailLevels)
            {
                Assert.ThrowsException<ArgumentException>(() =>
                {
                    var data = new AudioViewDataBuffer(0, (UInt32)40, (UInt32) detailLevels);
                });
            }

            [TestMethod]
            [TestCategory("AudioViewDataBuffer")]
            public async Task AudioViewDataBuffer_CreateFromFileAsync()
            {
                var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///TestContent/test_signal.mp3"));
                var dataOp = AudioViewDataBuffer.CreateFromFileAsync(testFile,1,new uint[] { 0 },null);
                dataOp.Progress += (op,progress) => 
                {
                    System.Diagnostics.Debug.WriteLine($"{progress}%");
                };
                var data = await dataOp;
            } 
        } */
    }
