#include "blog.h"
/*
 *     QxOrm_Impl.h，它的作用是检测内存泄露。如果使用 QxMemLeak 模块或 boost::serialization 引擎，
 * 应该在所有的 *.cpp 中包含它；否则，它便是可选的（非必须）
 */
#include "QxOrm_Impl.h"

//和 QX_REGISTER_HPP_QX_BLOG  相同，QX_REGISTER_CPP_QX_BLOG 宏也是必需的，用于将 Blog 类注册到 QxOrm 的上下文中。
QX_REGISTER_CPP_QX_BLOG(Blog)

namespace qx {
    //在 **.cpp 文件中，需要实现 qx::register_class()，它是一个设置函数：
    template <> void register_class(QxClass<Blog> & t) {

        // 注册 Blog::blogId <=> 数据库中的主键
        t.id(& Blog::blogId, "blog_id");

        //字段映射
        t.data(& Blog::text, "blog_text");
        t.data(& Blog::createTime, "date_creation");
        //多对1关系需定义如下
        t.relationManyToOne(& Blog::author, "author_id");
        //表与表关系：1对多，参数一：多属性，参数二：属性类型，参数三：1的主键
        t.relationOneToMany(& Blog::comments, "ListComment", "blog_id");
        /*
         * 表与表关系：多对多
         *     是取出两个表（blog 和 category）的主键再构建另外一个表，“category_blog”，category_id，blog_id
         * 都是表 “category_blog” 字段，外键关联表 blog 和 category
         *
         */
        t.relationManyToMany(& Blog::categories, "ListCategory", "category_blog", "blog_id", "category_id");
    }
}

Blog::Blog() : blogId(0)
{

}

Blog::~Blog()
{

}
