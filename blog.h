#ifndef BLOG_H
#define BLOG_H

#include "precompiled.h"

#include "author.h"
#include "category.h"
#include "comment.h"

/**
 * @brief 博客
 */
class Blog
{
public:
    long blogId;
    QString text;
    QDateTime createTime;
    //多对1关系
    AuthorPtr author;
    //1对多关系
    ListComment comments;
    //多对多关系
    ListCategory categories;
public:
    Blog();
    virtual ~Blog();
};
/*
 * 用于将 Blog 类注册到 QxOrm 的上下文中：
 * 参数一：表示要注册的当前类 - Blog；
 * 参数二：基类，如果没有基类，则使用 qx::trait::no_base_class_defined；
 * 参数三：用于序列化的类版本。
 */
QX_REGISTER_HPP_QX_BLOG(Blog, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Blog> BlogPtr;
typedef QVector<BlogPtr> ListBlog;

#endif // BLOG_H
