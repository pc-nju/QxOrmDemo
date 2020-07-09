#include "comment.h"

#include "blog.h"
#include "QxOrm_Impl.h"

QX_REGISTER_CPP_QX_BLOG(Comment)

namespace qx {
    template <> void register_class(QxClass<Comment> & t) {

        //主键
        t.id(& Comment::commentId, "comment_id");

        //字段映射
        t.data(& Comment::text, "comment_text");
        t.data(& Comment::createTime, "date_creation");

        //多对1关系需定义如下
        t.relationManyToOne(& Comment::blog, "blog_id");
    }
}

Comment::Comment() : commentId(0)
{

}

Comment::~Comment()
{

}
