#ifndef COMMENT_H
#define COMMENT_H

#include "precompiled.h"
#include <QSharedPointer>

class Blog;

/**
 * @brief 博客评论
 */
class Comment
{
public:
    //多对1关系需定义如下
    typedef QSharedPointer<Blog> BlogPtr;

    long commentId;
    QString text;
    QDateTime createTime;
    //多对1关系需定义如下属性
    BlogPtr blog;
public:
    Comment();
    virtual ~Comment();
};

QX_REGISTER_HPP_QX_BLOG(Comment, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Comment> CommentPtr;
typedef QList<CommentPtr> ListComment;

#endif // COMMENT_H
