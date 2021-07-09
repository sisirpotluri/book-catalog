//
//  main.cpp
//  Book List
//
//  Created by Sisir Potluri on 5/22/21.
//

#include <iostream>
#include <string>
#include "Machine.h"

using namespace std;

int main(int argc, const char * argv[]) {
    Machine machine;
    machine.execute();
    machine.update_recommendations();
    
    return 0;
}
