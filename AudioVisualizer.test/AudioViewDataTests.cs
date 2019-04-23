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
        public void AudioViewData_ctor_ZeroDownsampleThrows()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new AudioViewData(1024, 1, 0);
                }
                );
        }

        [DataTestMethod]
        [DataRow(1)]
        [DataRow(3)]
        [DataRow(44)]
        [TestCategory("AudioViewData")]
        public void AudioViewData_ctor_Downsample(int downsample)
        {
            var data = new AudioViewData(1024, 1, (uint) downsample);
            Assert.AreEqual((uint)downsample, data.Downsample);
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
            sut = new AudioViewData(4096, expectedLevels, 5);
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
        public void AudioViewData_LevelCount()
        {
            Assert.AreEqual(expectedLevels, (uint)sut.Count);
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_LevelElementsCountIsZero()
        {
            Assert.IsTrue(sut.All((list) => { return !list.Any(); }));
        }



        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_Add_1()
        {
            sut.Add(new float[] { 1.0f }.AsEnumerable());
            Assert.IsFalse(sut[0].Any());
            Assert.IsFalse(sut[1].Any());
            Assert.IsFalse(sut[2].Any());
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_Add_5()
        {
            sut.Add(Enumerable.Range(1, 5).Select( (i) => (float) i));
            CollectionAssert.AreEqual(new float[] { (float)Math.Sqrt(11) },sut[0].ToArray());
            Assert.IsFalse(sut[1].Any());
            Assert.IsFalse(sut[2].Any());
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_Add_25()
        {
            sut.Add(Enumerable.Range(1,20).Select((i) => (float)i));
            CollectionAssert.AreEqual(new float[] { (float)Math.Sqrt(11), (float)Math.Sqrt(66), (float)Math.Sqrt(171), (float)Math.Sqrt(326) }, sut[0].ToArray());
            CollectionAssert.AreEqual(new float[] { (float)Math.Sqrt(143.5) }, sut[1].ToArray());
            Assert.IsFalse(sut[2].Any());
        }
    }

    [TestClass]
    public class AudioViewDataLoadTests
    {
        [TestMethod]
        [TestCategory("AudioViewData")]
        public async Task AudioViewData_Load()
        {
            var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///TestContent/test_signal.mp3"));
            var stream = await testFile.OpenAsync(FileAccessMode.Read);

            var data = new AudioViewData(0, 3, 48);

            List<TimeSpan> progress = new List<TimeSpan>();

            var loadOp = data.LoadFromStreamAsync(stream);
            loadOp.Progress = new Windows.Foundation.AsyncActionProgressHandler<TimeSpan>((a,p) => progress.Add(p));
            await loadOp;
            Logger.LogMessage("Hello {0}",progress);
        }

    }

}
