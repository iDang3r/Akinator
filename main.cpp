#include <iostream>
#include <cassert>
#include <vector>
#include <cstdio>
using std::cin;
using std::cout;
using std::endl;

#include <sys/stat.h>

#include "super_define.h"
#include "dump.h"
#include "Akinator_Tree.h"

int main() {

    Akinator_Tree Q;

    Q.get_tree_from_file(X_("input.txt"));

    Q.game();


    Q.dump_picture();

    if (Q.edited)
        Q.put_tree_in_file(X_("input.txt"));

//    Q.put_tree_in_file(X_("out_tree.txt"));

    cout << "END" << endl;
    return 0;
}