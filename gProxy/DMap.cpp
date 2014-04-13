#include "StdAfx.h"
#include "DMap.h"

map<UInt16, string> DMap::MapID2Name;
map<string, gMap*> DMap::AllMaps; 
vector<string> DMap::MapNames;