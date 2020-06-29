#include "utils/FileUtils.hpp"
#include "pugixml.hpp"
#include <iostream>
#include <string>

using namespace Utils;
using std::string;

static int test_count = 0;
static int pass_count = 0;

#define AssertEqual(a, b)               \
    do {                                \
        auto x = a;                     \
        auto y = b;                     \
        ++test_count;                   \
        if ((x) == (y)) ++pass_count;   \
        else std::cout << '[' << test_count << "] expect " << y << " but get " << x << std::endl; \
    } while (0)


#define TESTBEGIN(name)                 \
    void test##name() {


#define TESTENG(name)                       \
        printf("Test %s Finish: %d/%d pass!\n", #name, pass_count, test_count);  \
        test_count = 0;                 \
        pass_count = 0;                 \
    }

#define TEST(name)                      \
    test##name()

TESTBEGIN(FileUtils)
    AssertEqual(path_norm("a\\b\\c"), "a/b/c");
    AssertEqual(path_norm("a//b//c"), "a/b/c");
    AssertEqual(path_norm("./a/./b/./c"), "a/b/c");
    AssertEqual(path_norm("a/../b/../c"), "c");
    AssertEqual(path_norm("a/../b/.."), ".");
    AssertEqual(path_norm("./../"), "..");
    AssertEqual(path_norm("./."), ".");
    AssertEqual(path_norm("../b/../.."), "../..");
    AssertEqual(path_norm("/../a/b/c"), "/a/b/c");

    AssertEqual(path_ensure_dir("./a/b", "res/Package"), "res/Package/a/b");
    AssertEqual(path_ensure_dir("./res/Package/a/b", "res/Package"), "res/Package/a/b");
    AssertEqual(path_ensure_dir("a/b", "."), "a/b");
    AssertEqual(path_ensure_dir("../a/b", ".."), "../a/b");
    AssertEqual(path_ensure_dir("a/b", ".."), "../a/b");
TESTENG(FileUtils)


TESTBEGIN(XmlUtils)
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("package/config/test.xml");
    std::cout << "Load result: " << result.description() << std::endl;
    AssertEqual(doc.child("mesh").attribute("name").value(), string("mesh_root"));
    pugi::xml_node mesh = doc.child("mesh");
    int index = 1;
    for (pugi::xml_node node = mesh.first_child(); node; node = node.next_sibling(), index++) {
        AssertEqual(node.attribute("attr").value(), string("value") + std::to_string(index));
    }
    AssertEqual(doc.child("mesh").child("node").attribute("attr").value(), string("value1"));
TESTENG(XmlUtils)


int main() {
    TEST(FileUtils);
    TEST(XmlUtils);

    getchar();
    return 0;
}