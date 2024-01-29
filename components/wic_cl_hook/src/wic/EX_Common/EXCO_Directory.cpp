#include <Ice/Ice.h>
#include "EXCO_Directory.h"

const Ice *EXCO_Directory::GetDirectoryRoot()
{
	return ourInstance->myDirectoryRoot;
}