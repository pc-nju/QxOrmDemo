#ifndef AUTHOR_H
#define AUTHOR_H


#include "precompiled.h"

#include <QSharedPointer>

class Blog;
/**
 * @brief 作者
 */
//class QX_BLOG_DLL_EXPORT Author
class Author
{
public:
    //1对多关系需定义如下
    typedef QSharedPointer<Blog> BlogPtr;
    typedef QVector<BlogPtr> ListBlog;

    enum EnumSex{
        MALE,
        FEMALE,
        UNKOWN
    };

    QString authorId;
    QString name;
    QDate birthday;
    EnumSex sex;
    //1对多关系需定义如下属性
    ListBlog blogs;

public:
    Author();
    virtual ~Author();
    int age() const;
};

//主键不是整数类型的时候使用
QX_REGISTER_PRIMARY_KEY(Author, QString)
/*
 * 用于将 Author 类注册到 QxOrm 的上下文中：
 * 参数一：表示要注册的当前类 - Author；
 * 参数二：基类，如果没有基类，则使用 qx::trait::no_base_class_defined；
 * 参数三：用于序列化的类版本。
 */
QX_REGISTER_HPP_QX_BLOG(Author, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Author> AuthorPtr;
typedef qx::QxCollection<QString, AuthorPtr> ListAuthor;

#endif // AUTHOR_H
