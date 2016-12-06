import CustomLibrary.myTestRunner.MyTestRunner
import unittest

class FooTest(unittest.TestCase):

    def setUp(self):
        self.a = 1

    def testPass(self):
        self.a = self.a + 1
        self.assertEqual(2, self.a)

    def testFail(self):
        self.a = self.a + 1
        self.assertEqual(3, self.a)
        

if __name__=='__main__':
    unittest.main(testRunner=CustomLibrary.myTestRunner.MyTestRunner())