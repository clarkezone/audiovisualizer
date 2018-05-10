using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AudioVisualizer.test
{
    [TestClass]
    public class testScalarData
    {
        [TestMethod]
        public void CreateEmpty()
        {
            var obj = ScalarData.CreateEmpty(2);
            Assert.IsNotNull(obj);
            Assert.IsInstanceOfType(obj, typeof(ScalarData));
            Assert.AreEqual(ScaleType.Linear,obj.AmplitudeScale);
        }
    }
}
