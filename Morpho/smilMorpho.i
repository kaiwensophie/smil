

%{
/* Includes the header in the wrapper code */
#include "DTypes.hpp"
#include "DMorphoBase.hpp"
#include "DMorphoGeodesic.hpp"
#include "DMorphoExtrema.hpp"
%}
 

%extend StrElt
{
	std::string  __str__() {
	    std::stringstream os;
	    os << *self;
	    return os.str();
	}
}


%include "DTypes.hpp"
%include "DStructuringElement.h"
%include "DMorphoBase.hpp"
%include "DMorphoGeodesic.hpp"
%include "DMorphoExtrema.hpp"

//TEMPLATE_WRAP_FUNC(label);

TEMPLATE_WRAP_FUNC(dilate);
TEMPLATE_WRAP_FUNC(erode);
TEMPLATE_WRAP_FUNC(close);
TEMPLATE_WRAP_FUNC(open);
TEMPLATE_WRAP_FUNC(gradient);

TEMPLATE_WRAP_FUNC(geoDil);
TEMPLATE_WRAP_FUNC(geoEro);
TEMPLATE_WRAP_FUNC(build);
TEMPLATE_WRAP_FUNC(dualBuild);

TEMPLATE_WRAP_FUNC(hMinima);
TEMPLATE_WRAP_FUNC(hMaxima);
TEMPLATE_WRAP_FUNC(minima);
TEMPLATE_WRAP_FUNC(maxima);
