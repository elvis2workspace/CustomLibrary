# -*- coding: utf-8 -*-


def salesgirl(discount):
    def expense(method):
        def serve(*args):
            print "Salesgirl:Hello, what do you want?", method.__name__
            result_value = method(*args)
            print result_value
            if result_value:
                print "Salesgirl: This shirt is 50$.As an old user, we promised to discount at %d%%" % discount
            else:
                print "Salesgirl: Well, how about trying another style?"

            return result_value
        return serve
    return expense


@salesgirl(50)
def try_this_shirt(size):
    if size < 35:
        print "I: %d inches is to small to me" % size
        return False
    else:
        print "I:%d inches is just enough" % size
        return True


if __name__ == '__main__':

    result = try_this_shirt(38)
    print "Mum:do you want to buy this?", result
