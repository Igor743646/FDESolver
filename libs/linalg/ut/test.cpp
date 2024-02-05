#include <../matrix.hpp>
#include <cassert>

class TBaseTest {
public:

    virtual int DoRun() = 0;

    int Run() {
        return DoRun();
    }
};

class TTestMatrixInit : public TBaseTest {
public:

    int DoRun() {
        NLinalg::TMatrix m(3);

        for (usize i = 0; i < 3 * 3; i++) {
            assert(m[i / 3][i % 3] == 0.0);
        }

        return 0;
    }
};

int main() {

    TBaseTest* test = new TTestMatrixInit();

    int result = 0;
    
    try {
        int result = test->Run();

        if (result != 0) {
            throw result;
        }
    } catch(...) {
        return 1;
    }
    
    
    delete test;
    return 0;
}