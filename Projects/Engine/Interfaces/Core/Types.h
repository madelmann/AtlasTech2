
#ifndef _Interfaces_Core_Types_h_
#define _Interfaces_Core_Types_h_


// Library includes
#include <list>
#include <set>

// Project includes
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


namespace Core {


typedef AEint AEid;


const AEid invalidID = -1;


typedef std::list<AEid> IdList;
typedef std::set<AEid> IdSet;


}


#endif
