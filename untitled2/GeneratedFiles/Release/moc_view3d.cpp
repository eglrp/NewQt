/****************************************************************************
** Meta object code from reading C++ file 'view3d.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../view3d.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view3d.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_View3D_t {
    QByteArrayData data[16];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_View3D_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_View3D_t qt_meta_stringdata_View3D = {
    {
QT_MOC_LITERAL(0, 0, 6), // "View3D"
QT_MOC_LITERAL(1, 7, 18), // "RotateAngleChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "AxisChanged"
QT_MOC_LITERAL(4, 39, 6), // "Render"
QT_MOC_LITERAL(5, 46, 15), // "OnWindowChanged"
QT_MOC_LITERAL(6, 62, 13), // "QQuickWindow*"
QT_MOC_LITERAL(7, 76, 7), // "pWindow"
QT_MOC_LITERAL(8, 84, 7), // "Release"
QT_MOC_LITERAL(9, 92, 27), // "get_windows_hwnd_from_title"
QT_MOC_LITERAL(10, 120, 4), // "HWND"
QT_MOC_LITERAL(11, 125, 5), // "title"
QT_MOC_LITERAL(12, 131, 11), // "rotateAngle"
QT_MOC_LITERAL(13, 143, 4), // "axis"
QT_MOC_LITERAL(14, 148, 6), // "OpenGL"
QT_MOC_LITERAL(15, 155, 10) // "CWS3DShow*"

    },
    "View3D\0RotateAngleChanged\0\0AxisChanged\0"
    "Render\0OnWindowChanged\0QQuickWindow*\0"
    "pWindow\0Release\0get_windows_hwnd_from_title\0"
    "HWND\0title\0rotateAngle\0axis\0OpenGL\0"
    "CWS3DShow*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_View3D[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       3,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   46,    2, 0x09 /* Protected */,
       5,    1,   47,    2, 0x09 /* Protected */,
       8,    0,   50,    2, 0x09 /* Protected */,

 // methods: name, argc, parameters, tag, flags
       9,    1,   51,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // methods: parameters
    0x80000000 | 10, QMetaType::QString,   11,

 // properties: name, type, flags
      12, QMetaType::QReal, 0x00495103,
      13, QMetaType::QVector3D, 0x00495103,
      14, 0x80000000 | 15, 0x0009500b,

 // properties: notify_signal_id
       0,
       1,
       0,

       0        // eod
};

void View3D::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        View3D *_t = static_cast<View3D *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RotateAngleChanged(); break;
        case 1: _t->AxisChanged(); break;
        case 2: _t->Render(); break;
        case 3: _t->OnWindowChanged((*reinterpret_cast< QQuickWindow*(*)>(_a[1]))); break;
        case 4: _t->Release(); break;
        case 5: { HWND _r = _t->get_windows_hwnd_from_title((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< HWND*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QQuickWindow* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (View3D::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View3D::RotateAngleChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (View3D::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View3D::AxisChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        View3D *_t = static_cast<View3D *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->RotateAngle(); break;
        case 1: *reinterpret_cast< QVector3D*>(_v) = _t->Axis(); break;
        case 2: *reinterpret_cast< CWS3DShow**>(_v) = _t->readOpenGL(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        View3D *_t = static_cast<View3D *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setRotateAngle(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setAxis(*reinterpret_cast< QVector3D*>(_v)); break;
        case 2: _t->InitOpenGL(*reinterpret_cast< CWS3DShow**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject View3D::staticMetaObject = {
    { &QQuickItem::staticMetaObject, qt_meta_stringdata_View3D.data,
      qt_meta_data_View3D,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *View3D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *View3D::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_View3D.stringdata0))
        return static_cast<void*>(const_cast< View3D*>(this));
    if (!strcmp(_clname, "CWS3DShow"))
        return static_cast< CWS3DShow*>(const_cast< View3D*>(this));
    return QQuickItem::qt_metacast(_clname);
}

int View3D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void View3D::RotateAngleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void View3D::AxisChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE