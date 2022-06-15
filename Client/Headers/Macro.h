#ifndef MACRO_H__
#define MACRO_H__


//#define COLLISION_CHECK
//#define BONE_TEST
#define ENEMY_BONE_TEST



#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG 

#endif // MACRO_H__

