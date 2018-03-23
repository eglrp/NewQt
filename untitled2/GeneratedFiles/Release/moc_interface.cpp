/****************************************************************************
** Meta object code from reading C++ file 'interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_InterFace_t {
    QByteArrayData data[26];
    char stringdata0[332];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InterFace_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InterFace_t qt_meta_stringdata_InterFace = {
    {
QT_MOC_LITERAL(0, 0, 9), // "InterFace"
QT_MOC_LITERAL(1, 10, 12), // "imageRefresh"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 15), // "registerSuccess"
QT_MOC_LITERAL(4, 40, 14), // "registerFailed"
QT_MOC_LITERAL(5, 55, 13), // "personChanged"
QT_MOC_LITERAL(6, 69, 12), // "swPersonInfo"
QT_MOC_LITERAL(7, 82, 6), // "person"
QT_MOC_LITERAL(8, 89, 16), // "recognizeSuccess"
QT_MOC_LITERAL(9, 106, 13), // "swPersonInfo&"
QT_MOC_LITERAL(10, 120, 15), // "recognizeFailed"
QT_MOC_LITERAL(11, 136, 16), // "calibStepSuccess"
QT_MOC_LITERAL(12, 153, 15), // "calibStepFailed"
QT_MOC_LITERAL(13, 169, 16), // "calibUnitSuccess"
QT_MOC_LITERAL(14, 186, 15), // "calibUnitFailed"
QT_MOC_LITERAL(15, 202, 4), // "Init"
QT_MOC_LITERAL(16, 207, 10), // "StartVideo"
QT_MOC_LITERAL(17, 218, 15), // "EnterRegisterUi"
QT_MOC_LITERAL(18, 234, 15), // "TriggerRegister"
QT_MOC_LITERAL(19, 250, 16), // "EnterRecognizeUi"
QT_MOC_LITERAL(20, 267, 16), // "TriggerRecognize"
QT_MOC_LITERAL(21, 284, 12), // "EnterCalibUi"
QT_MOC_LITERAL(22, 297, 16), // "TriggerCalibStep"
QT_MOC_LITERAL(23, 314, 4), // "unit"
QT_MOC_LITERAL(24, 319, 9), // "SetPerson"
QT_MOC_LITERAL(25, 329, 2) // "id"

    },
    "InterFace\0imageRefresh\0\0registerSuccess\0"
    "registerFailed\0personChanged\0swPersonInfo\0"
    "person\0recognizeSuccess\0swPersonInfo&\0"
    "recognizeFailed\0calibStepSuccess\0"
    "calibStepFailed\0calibUnitSuccess\0"
    "calibUnitFailed\0Init\0StartVideo\0"
    "EnterRegisterUi\0TriggerRegister\0"
    "EnterRecognizeUi\0TriggerRecognize\0"
    "EnterCalibUi\0TriggerCalibStep\0unit\0"
    "SetPerson\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InterFace[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,
       3,    0,  110,    2, 0x06 /* Public */,
       4,    0,  111,    2, 0x06 /* Public */,
       5,    1,  112,    2, 0x06 /* Public */,
       8,    1,  115,    2, 0x06 /* Public */,
      10,    0,  118,    2, 0x06 /* Public */,
      11,    0,  119,    2, 0x06 /* Public */,
      12,    0,  120,    2, 0x06 /* Public */,
      13,    0,  121,    2, 0x06 /* Public */,
      14,    0,  122,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      15,    0,  123,    2, 0x02 /* Public */,
      16,    0,  124,    2, 0x02 /* Public */,
      17,    0,  125,    2, 0x02 /* Public */,
      18,    0,  126,    2, 0x02 /* Public */,
      19,    0,  127,    2, 0x02 /* Public */,
      20,    0,  128,    2, 0x02 /* Public */,
      21,    0,  129,    2, 0x02 /* Public */,
      22,    1,  130,    2, 0x02 /* Public */,
      24,    1,  133,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::QString,   25,

       0        // eod
};

void InterFace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InterFace *_t = static_cast<InterFace *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageRefresh(); break;
        case 1: _t->registerSuccess(); break;
        case 2: _t->registerFailed(); break;
        case 3: _t->personChanged((*reinterpret_cast< const swPersonInfo(*)>(_a[1]))); break;
        case 4: _t->recognizeSuccess((*reinterpret_cast< swPersonInfo(*)>(_a[1]))); break;
        case 5: _t->recognizeFailed(); break;
        case 6: _t->calibStepSuccess(); break;
        case 7: _t->calibStepFailed(); break;
        case 8: _t->calibUnitSuccess(); break;
        case 9: _t->calibUnitFailed(); break;
        case 10: { bool _r = _t->Init();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->StartVideo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->EnterRegisterUi();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->TriggerRegister();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->EnterRecognizeUi();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->TriggerRecognize();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->EnterCalibUi();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->TriggerCalibStep((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: _t->SetPerson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::imageRefresh)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::registerSuccess)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::registerFailed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)(const swPersonInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::personChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)(swPersonInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::recognizeSuccess)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::recognizeFailed)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::calibStepSuccess)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::calibStepFailed)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::calibUnitSuccess)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (InterFace::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterFace::calibUnitFailed)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject InterFace::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_InterFace.data,
      qt_meta_data_InterFace,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *InterFace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InterFace::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_InterFace.stringdata0))
        return static_cast<void*>(const_cast< InterFace*>(this));
    return QObject::qt_metacast(_clname);
}

int InterFace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void InterFace::imageRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void InterFace::registerSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void InterFace::registerFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void InterFace::personChanged(const swPersonInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void InterFace::recognizeSuccess(swPersonInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void InterFace::recognizeFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void InterFace::calibStepSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void InterFace::calibStepFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void InterFace::calibUnitSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}

// SIGNAL 9
void InterFace::calibUnitFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
