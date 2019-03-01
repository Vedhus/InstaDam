#include "selectgroup.h"

SelectGroup::SelectGroup(){

}

SelectGroup::~SelectGroup(){

}

void SelectGroup::addItem(SelectItem *item){
    items.push_back(item);
}
