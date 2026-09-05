// helper utility to help translate SimpleV's java algorithms to c++

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>

std::pair<std::regex, std::string> rules[] = {
    {std::regex("([^v])array"), "$1arr"},
    {std::regex("int\\[\\] arr"), "varray &arr"},
    {std::regex("highlight.swap\\(arr, (.*?), (.*?)\\)"), "std::swap(arr[$1], arr[$2])"},
    {std::regex("highlight.read\\(arr, (.*?)\\)"), "arr[$1]"},
    {std::regex("highlight.write\\(arr, (.*?), (.*?)\\)"), "arr[$1] = $2"},
    {std::regex("highlight.rotate"), "rotate"},
    {std::regex("highlight.reverse"), "reverse"},
    {std::regex("int\\[\\]"), "std::vector<int>"},
    {std::regex("= new int\\[(.*?)\\]"), "($1)"},
    {std::regex("public "), ""},
    {std::regex("private "), ""},
    {std::regex("boolean"), "bool"},
    {std::regex("Math."), ""},
    {std::regex("min\\("), "std::min("},
    {std::regex("max\\("), "std::max("},
    {std::regex("ArrayList<Integer>"), "std::vector<int>"},
    {std::regex(" ?= new ArrayList<>()"), ""},
    {std::regex("\\.add"), ".push_back"},
    {std::regex("\\.get\\((.*?)\\)"), "[$1]"},
    {std::regex("len"), "arr.len"},
    {std::regex(">>>="), ">>="},
    {std::regex(">>>"), ">>"},
};

int main() {
    for (const auto& entry : std::filesystem::directory_iterator("./")) {
        if (std::filesystem::is_regular_file(entry)) {
            if (entry.path().extension() == ".java") {
                std::string str = entry.path().filename().string();
                std::cout << str << "\n";
                std::ifstream file(str);
                std::stringstream buf;
                buf << file.rdbuf();
                std::string contents = buf.str();
                file.close();
                std::cout << "test\n";
                contents.erase(0, contents.find_first_of("{")+1);
                std::cout << "test\n";
                contents.erase(0, contents.find_first_of("}")+2);
                std::cout << "test\n";
                int pos = contents.find_first_of("@");
                std::cout << "test\n";
                std::cout << contents.substr(pos).substr(0, contents.substr(pos).find_first_of("{")) << "\n";
                contents.erase(pos, contents.substr(pos).find_first_of("{")+1);
                std::cout << "test\n";
                contents.insert(pos, "extern \"C\" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {");
                std::cout << "test\n";
                contents.erase(contents.length()-2);
                std::cout << "test\n";
                for (auto &rule : rules) contents = std::regex_replace(contents, rule.first, rule.second);
                if (contents.contains("rotate")) contents.insert(0, "int rotate(varray &arr, int l, int mid, int r) {\n    int a = l;\n    int b = mid;\n    int c = mid;\n    int d = r;\n    if (mid-l > r-mid) {\n        int loop = (r-mid)/2;\n        while (loop-- > 0) {\n            b--; d--;\n            int temp = arr[b];\n            arr[b] = arr[a];\n            arr[a] = arr[c];\n            arr[c] = arr[d];\n            arr[d] = temp;\n            a++; c++;\n        }\n        loop = (b-a)/2;\n        while (loop-- > 0) {\n            b--; d--;\n            int temp = arr[b];\n            arr[b] = arr[a];\n            arr[a] = arr[d];\n            arr[d] = temp;\n            a++;\n        }\n        loop = (d-a)/2;\n        while (loop-- > 0) {\n            d--;\n            int temp = arr[a];\n            arr[a] = arr[d];\n            arr[d] = temp;\n            a++;\n        }\n    } else if (mid-l < r-mid) {\n        int loop = (mid-l)/2;\n        while (loop-- > 0) {\n            b--; d--;\n            int temp = arr[b];\n            arr[b] = arr[a];\n            arr[a] = arr[c];\n            arr[c] = arr[d];\n            arr[d] = temp;\n            a++; c++;\n        }\n        loop = (d-c)/2;\n        while (loop-- > 0) {\n            d--;\n            int temp = arr[c];\n            arr[c] = arr[d];\n            arr[d] = arr[a];\n            arr[a] = temp;\n            a++; c++;\n        }\n        loop = (d-a)/2;\n        while (loop-- > 0) {\n            d--;\n            int temp = arr[a];\n            arr[a] = arr[d];\n            arr[d] = temp;\n            a++;\n        }\n    } else {\n        int loop = mid-l;\n        while (loop-- > 0) {\n            int temp = arr[a];\n            arr[a] = arr[b];\n            arr[b] = temp;\n            a++; b++;\n        }\n    }\n    return l+r-mid;\n}\n\n");
                if (contents.contains("reverse")) contents.insert(0, "void reverse(varray &arr, int l, int r) {\n    for (int i = l; i < (l+r)/2; i++) {\n        std::swap(arr[i], arr[r-i-1+l]);\n    }\n}\n\n");
                contents.insert(0, "#include <variant>\n#include <iostream>\n#include <cmath>\n#include \"../varray.cpp\"\n\n");
                std::cout << "done\n";
                std::ofstream outfile(str, std::ios::trunc);
                outfile << contents;
                outfile.close();
                std::string out = str.substr(0, str.find_first_of("."));
                for (char &c : out) {
                    c = std::tolower(c);
                }
                std::filesystem::rename(str, out+".cpp");
            }
        }
    }
    return 0;
}