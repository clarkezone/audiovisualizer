using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace AudioVisualizer.test
{
    [TestClass]
    public class CreateResourcesArgsTests
    {
        AudioVisualizer.CreateResourcesEventArgs sut;
        Object fakeCreator = new Object();

        [TestInitialize]
        public void TestInit()
        {
            sut = new CreateResourcesEventArgs(CreateResourcesReason.New, fakeCreator);
        }

        [TestMethod]
        public void CreateResourcesEventArgs_IsReasonNew()
        {
            Assert.AreEqual(CreateResourcesReason.New, sut.Reason);
        }
        [TestMethod]
        public void CreateResourcesEventArgs_IsCreatorObjectCorrect()
        {
            Assert.AreEqual(fakeCreator, sut.ResourceCreator);
        }
    }
}
