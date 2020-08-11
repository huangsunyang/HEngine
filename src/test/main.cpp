#include "utils/FileUtils.hpp"
#include "pugixml.hpp"
#include <pybind11/embed.h>
#include <iostream>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "rapidjson/document.h"
#include <fstream>
#include "glm/glm.hpp"
#include "3D/Skeleton.hpp"

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
    struct test##name {                 \
        test##name() {


#define TESTEND(name)                       \
            printf("------- [Test %s Finish]: %d/%d pass! -------\n", #name, pass_count, test_count);  \
            test_count = 0;                 \
            pass_count = 0;                 \
        }                                   \
    }                                       \
    static test##name;


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
TESTEND(FileUtils)


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
TESTEND(XmlUtils)


TESTBEGIN(Pybind11)
    namespace py = pybind11;
    py::scoped_interpreter p;
    py::list sys_path = py::module::import("sys").attr("path");
    for (auto path: sys_path) {
        py::print(path);
    }
TESTEND(Pybind11)


TESTBEGIN(Freetype)
    FT_Library ft;
    FT_Face face;
TESTEND(FreeType)


TESTBEGIN(RapidJson)
    rapidjson::Document d;
    std::fstream t("package/spine/spineboy/spineboy-pro.json", std::ios::in);
    std::string ret((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    d.Parse(ret.c_str());
    AssertEqual(d["skeleton"]["spine"].GetString(), string("3.8.55"));
TESTEND(RapidJson)


TESTBEGIN(glm)
    glm::vec3 a(1.0f, 1.0f, 1.0f);
    glm::vec3 b(3.0f, 3.0f, 3.0f);
    a *= b;
    AssertEqual(a.x, 3.0f);
    AssertEqual(glm::clamp(1, 2, 3), 2);
    Skeleton sk;
    sk.loadFromFile("package/res/test.skel.txt");
    AssertEqual(sk.getBoneTree()->getName(), "root");
TESTEND(glm)


int main() {
    getchar();
    return 0;
}