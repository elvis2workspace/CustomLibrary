class TestClassMethod(object):

    METHOD = 'method hoho'

    def __init__(self):
        self.name = 'leon'

    #  类实例方法
    def test1(self):
        print 'test1'
        print self.name

    # 类方法和静态方法都可以被类和类实例调用，类实例方法仅可以被类实例调用
    # 类方法的隐含调用参数是类，而类实例方法的隐含调用参数是类的实例，静态方法没有隐含调用参 
    @classmethod
    def test2(cls):
        print cls
        print 'test2'
        print TestClassMethod.METHOD
        print '----------------'

    @staticmethod
    def test3():
        print TestClassMethod.METHOD
        print 'test3'

if __name__ == '__main__':
    a = TestClassMethod()
    a.test1()
    a.test2()
    a.test3()
    TestClassMethod.test3()
