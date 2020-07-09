#include "category.h"

#include "blog.h"
#include "QxOrm_Impl.h"

QX_REGISTER_CPP_QX_BLOG(Category)

namespace qx {
    template <> void register_class(QxClass<Category> & t) {

        //主键
        t.id(& Category::categotyId, "category_id");

        //字段映射
        t.data(& Category::name, "name");
        t.data(& Category::desc, "description");

        /*
         * 表与表关系：多对多
         *     是取出两个表（blog 和 category）的主键再构建另外一个表，“category_blog”，category_id，blog_id
         * 都是表 “category_blog” 字段，外键关联表 blog 和 category
         *
         */
        t.relationManyToMany(& Category::blogs, "ListBlog", "category_blog", "category_id", "blog_id");
    }
}

Category::Category() : categotyId(0)
{

}

Category::~Category()
{

}
