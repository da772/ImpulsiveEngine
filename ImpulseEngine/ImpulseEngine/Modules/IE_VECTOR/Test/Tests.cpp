#include "IEVector/IEVector.hpp"
#include <iostream>
#include <cassert>

using namespace IEVector;


void vec2Tests();
void vec3Tests();
void vec4Tests();

int main() {
    vec2Tests();
    vec3Tests();
    vec4Tests();
    return 0;
}

void vec4Tests() {

 std::cout << std::endl << "Running Vector4 Tests: " << std::endl;
    Vector4f vc = {2,2,2,2};
    std::cout << "Testing reference operators" << std::endl;
    vc += 1;
    assert( (vc == Vector4f(3,3,3,3)) );
    
    vc *= 2;
    assert( (vc == Vector4f(6,6,6,6)) );

    vc /= 2;
    assert( (vc == Vector4f(3,3,3,3)) );
    
    vc -= 2;
    assert(vc == Vector4f(1,1,1, 1));

    vc++;
    assert(vc == Vector4f(2,2,2,2));

    vc--;
    assert(vc == Vector4f(1,1,1,1));
    std::cout << "Reference Operators Pass" << std::endl;

    assert(vc.x == vc.r);
    assert(vc.y == vc.g);
    assert(vc.z == vc.b);
    assert(vc.w == vc.a);
    assert(vc.s == vc.x);
    assert(vc.t == vc.y);
    assert(vc.p == vc.z);
    assert(vc.q == vc.w);

    
    Vector4f subVec(5,5, 6,4);
    std::cout << "Testing copy operators" << std::endl;
    Vector4f _subRes = subVec-vc;
    assert(_subRes == Vector4f(4,4,5,3));
    _subRes = _subRes+Vector4f(2,2,2,2);
    assert(_subRes == Vector4f(6,6,7,5));

    assert(Vector4f(1,2,3,4) + Vector2f(2,3) == Vector4f(3,5,3,4));
    assert(Vector4f(1,2,3,4) * Vector2f(2,3) == Vector4f(2,6,3,4));
    assert(Vector4f(1,2,3,4) - Vector2f(2,3) == Vector4f(-1,-1,3,4));
    assert(Vector4f(4,6,5,7) / Vector2f(2,3) == Vector4f(2,2,5,7));
    assert(Vector4<unsigned int>(0,1,1,3) == Vector4<unsigned int>(0,1,1,3));
    Vector4f dataTest(3,1,6.2, 8.2);
    assert(dataTest.data()[0] == dataTest.x && dataTest.data()[1] == dataTest.y && dataTest.data()[2] == dataTest.z && dataTest.data()[3] == dataTest.w);

    assert(Vector4f(5,5,10,6) + Vector3f(2,1,6) == Vector4f(7,6,16,6));
    assert(Vector4f(5,5,10,6) - Vector3f(2,1,6) == Vector4f(3,4,4,6));
    assert(Vector4f(5,5,10,6) * Vector3f(2,1,6) == Vector4f(10,5,60,6));
    assert(Vector4f(6,5,36,6) / Vector3f(2,1,6) == Vector4f(3,5,6,6));

    Vector4f copyTest = {2,6,1,9};
    copyTest += 3;
    assert(copyTest == Vector4f(5,9,4,12));
    copyTest -= 5;
    assert(copyTest == Vector4f(0,4,-1,7));
    copyTest *= 3;
    assert(copyTest == Vector4f(0,12,-3,21));
    copyTest /= 3;
    assert(copyTest == Vector4f(0,4, -1,7));
    copyTest += copyTest.xy();
    assert(copyTest == Vector4f(0,8,-1,7));
    copyTest += copyTest.xyz();
    assert(copyTest == Vector4f(0,16,-2,7));
    copyTest = 2.f * copyTest;
    assert(copyTest == Vector4f(0,32,-4,14));
    copyTest = copyTest/2.f;
    assert(copyTest == Vector4f(0,16,-2,7));
    copyTest = .5f + copyTest;
    assert(copyTest == Vector4f(.5f, 16.5f, -1.5f, 7.5f));
    std::cout << "Copy Operators Pass" << std::endl;
    
    std::cout << "Testing static functions" << std::endl;
    float dot = Vector4f::dot(copyTest,dataTest);
    vc= Vector4f::project(copyTest, dataTest);
    float distance = Vector4f::distance(copyTest,dataTest);
    Vector4f lerp = Vector4f::lerp(copyTest, dataTest, 5);
    vc = Vector4f::reflect(copyTest, dataTest);
    vc = 5.f*vc;
    std::cout << "Static Functions Pass" << std::endl;  
    std::cout << "Vector4 Tests Pass" << std::endl;
}

void vec3Tests() {
    std::cout << std::endl << "Running Vector3 Tests: " << std::endl;
    Vector3f vc = {2,2,2};
    std::cout << "Testing reference operators" << std::endl;
    vc += 1;
    assert( (vc == Vector3f(3,3,3)) );
    
    vc *= 2;
    assert( (vc == Vector3f(6,6,6)) );

    vc /= 2;
    assert( (vc == Vector3f(3,3,3)) );
    
    vc -= 2;
    assert(vc == Vector3f(1,1,1));

    vc++;
    assert(vc == Vector3f(2,2,2));

    vc--;
    assert(vc == Vector3f(1,1,1));


    assert(vc.x == vc.r);
    assert(vc.y == vc.g);
    assert(vc.z == vc.b);
    assert(vc.s == vc.x);
    assert(vc.t == vc.y);
    assert(vc.p == vc.z);

    std::cout << "Reference Operators Pass" << std::endl;
    Vector3f subVec(5,5, 6);
    std::cout << "Testing copy operators" << std::endl;
    Vector3f _subRes = subVec-vc;
    assert(_subRes == Vector3f(4,4,5));
    _subRes = _subRes+Vector3f(2,2, 2);
    assert(_subRes == Vector3f(6,6,7));

    assert(Vector3f(1,2,3) + Vector2f(2,3) == Vector3f(3,5,3));
    assert(Vector3f(1,2,3) * Vector2f(2,3) == Vector3f(2,6,3));
    assert(Vector3f(1,2,3) - Vector2f(2,3) == Vector3f(-1,-1,3));
    assert(Vector3f(4,6,5) / Vector2f(2,3) == Vector3f(2,2,5));
    assert(Vector3<unsigned int>(0,1,1) == Vector3<unsigned int>(0,1,1));
    Vector3f dataTest(3,1,6.2);
    assert(dataTest.data()[0] == dataTest.x && dataTest.data()[1] == dataTest.y && dataTest.data()[2] == dataTest.z);

    assert(Vector3f(5,5,10) + Vector3f(2,1,6) == Vector3f(7,6,16));
    assert(Vector3f(5,5,10) - Vector3f(2,1,6) == Vector3f(3,4,4));
    assert(Vector3f(5,5,10) * Vector3f(2,1,6) == Vector3f(10,5,60));
    assert(Vector3f(6,5,36) / Vector3f(2,1,6) == Vector3f(3,5,6));

    Vector3f copyTest = {2,6,1};
    copyTest += 3;
    assert(copyTest == Vector3f(5,9,4));
    copyTest -= 5;
    assert(copyTest == Vector3f(0,4,-1));
    copyTest *= 3;
    assert(copyTest == Vector3f(0,12,-3));
    copyTest /= 3;
    assert(copyTest == Vector3f(0,4, -1));
    copyTest += copyTest.xy();
    assert(copyTest == Vector3f(0,8,-1));
    copyTest = 5.f + copyTest;
    assert(copyTest == Vector3f(5,13,4));
    copyTest = 2.f* copyTest;
    assert(copyTest == Vector3f(10,26,8));

    std::cout << "Copy Operators Pass" << std::endl;
    std::cout << "Testing static functions" << std::endl;
    float dot = Vector3f::dot(copyTest,dataTest);
    vc= Vector3f::project(copyTest, dataTest);
    vc= Vector3f::cross(copyTest, dataTest);
    vc = Vector3f::reflect(copyTest, dataTest);
    vc = Vector3f::lerp(copyTest, dataTest, 5);
    float distance = Vector3f::distance(copyTest, dataTest);
    std::cout << "Static Functions Pass" << std::endl;  
    std::cout << "Vector3 Tests Pass" << std::endl;  

}

void vec2Tests() {
    std::cout << std::endl << "Running Vector2 Tests: " << std::endl;
    Vector2f vc = {2,2};
    std::cout << "Testing reference operators" << std::endl;
    vc += 1;
    assert(vc.x == 3 && vc.y == 3);
    vc *= 2;
    assert(vc.x == 6 && vc.y == 6);
    vc /= 2;
    assert(vc.x == 3 && vc.y == 3);
    vc -= 2;
    assert(vc.x == 1 && vc.y == 1);
    vc++;
    assert(vc.x == 2 && vc.y == 2);
    vc--;
    assert(vc.x == 1 && vc.y == 1);

    assert(vc.x == vc.r);
    assert(vc.y == vc.g);
    assert(vc.s == vc.x);
    assert(vc.t == vc.y);


    std::cout << "Reference Operators Pass" << std::endl;
    Vector2f subVec(5,5);
    std::cout << "Testing copy operators" << std::endl;
    Vector2f _subRes = subVec-vc;
    assert(_subRes.x == 4 && _subRes.y == 4);
    _subRes = _subRes+Vector2f(2,2);
    assert(_subRes.x == 6 && _subRes.y == 6);
    _subRes = _subRes*Vector2f(2,3);
    assert(_subRes.x == 12 && _subRes.y == 18);
    _subRes = _subRes/Vector2f(6,3);
    assert(_subRes.x == 2 && _subRes.y == 6);
    vc -= Vector2f(2,3);
    assert(vc.x == -1 && vc.y == -2);
    vc += Vector2f(4,6);
    assert(vc.x == 3 && vc.y == 4);
    vc *= Vector2f(2,3);
    assert(vc.x == 6 && vc.y == 12);
    vc /= Vector2f(6,4);
    assert(vc.x == 1 && vc.y == 3);
    vc = vc*6;
    assert(vc.x == 6 && vc.y == 18);
    vc = vc/3;
    assert(vc.x == 2 && vc.y == 6);
    vc = vc+2;
    assert(vc.x == 4 && vc.y == 8);
    vc = vc-4;
    assert(vc.x == 0 && vc.y == 4);
    vc = 5.f+vc;
    assert(vc == Vector2f(5,9));
    vc = 2.f*vc;
    assert(vc == Vector2f(10,18));
    std::cout << "Copy Operators Pass" << std::endl;
    vc += 3;
    std::cout << "Testing static functions" << std::endl;
    float dot = Vector2f::dot(vc,_subRes);
    vc= Vector2f::project(vc, _subRes);
    vc = Vector2f::reflect(_subRes, vc);
    vc = Vector2f::lerp(_subRes, vc, 5);
    float distance = Vector2f::distance(_subRes, vc);
    std::cout << vc.x << ", " << vc.y << " : " << vc.length() << " : " << vc.normalize().y << " - "  <<(vc.data()[1]) << std::endl;
    std::cout << "Static Functions Pass" << std::endl;

    std::cout << "Vector2 Tests Passed" << std::endl;
}
