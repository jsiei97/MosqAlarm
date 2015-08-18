#ifndef  __DEBUGDEFINES_H
#define  __DEBUGDEFINES_H

#define myOut()  qDebug() << "[" << __PRETTY_FUNCTION__ << __LINE__ << "]" << "D:"
#define myWarn() qDebug() << "[" << __PRETTY_FUNCTION__ << __LINE__ << "]" << "W:"
#define myErr()  qDebug() << "[" << __PRETTY_FUNCTION__ << __LINE__ << "]" << "E:"

#endif  // __DEBUGDEFINES_H 
