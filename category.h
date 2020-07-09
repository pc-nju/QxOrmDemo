#ifndef CATEGORY_H
#define CATEGORY_H

#include "precompiled.h"
#include <QSharedPointer>

class Blog;
/**
 * @brief 博客类别
 */
class Category
{
public:
    //多对多关系需定义如下
    typedef QSharedPointer<Blog> BlogPtr;
    typedef qx::QxCollection<long, BlogPtr> ListBlog;

    long categotyId;
    QString name;
    QString desc;
    //多对多关系需定义如下属性
    ListBlog blogs;

public:
    Category();
    virtual ~Category();
};

QX_REGISTER_HPP_QX_BLOG(Category, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Category> CategoryPtr;
typedef qx::QxCollection<long, CategoryPtr> ListCategory;

#endif // CATEGORY_H
