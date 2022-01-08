#include "observateur.h"

Etat transition (Etat e, Alphabet alp)
{
 
  switch (alp){
   case A :
      switch (e){
      	case INIT : 
		e = ERR;
		break;
      	case ERR : 
		e = ERR; 
		break;
      	case MES : 
		e = INIT; 
		break;
      }
      break;
   case G :
      switch (e){
      	case INIT : 
		e = INIT;
		break;
      	case ERR : 
		e = ERR; 
		break;
      	case MES : 
		e = INIT; 
		break;
      }
      break;
   case M :
      switch (e){
      	case INIT : 
		e = MES;
		break;
        case ERR : 
		e = ERR; 
		break;
        case MES :
		e = MES; 
		break;
        }
	break;
   case D :
      switch (e){
      	case INIT : 
		e = INIT;
		break;
      	case ERR : 
		e = ERR; 
		break;
      	case MES : 
		e = INIT; 
		break;
      }
      break;
  }
  return e;
}   

Etat initialiser_etat (Etat e)
{
    return INIT;
}

