#include "author.h"

#include "blog.h"
/*
 *     QxOrm_Impl.h，它的作用是检测内存泄露。如果使用 QxMemLeak 模块或 boost::serialization 引擎，
 * 应该在所有的 *.cpp 中包含它；否则，它便是可选的（非必须）
 */
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_BLOG(Author)

namespace qx {
    template <> void register_class(QxClass<Author> & t) {

        //主键
        t.id(& Author::authorId, "author_id");

        //字段映射
        t.data(& Author::name, "name");
        t.data(& Author::birthday, "birthday");
        t.data(& Author::sex, "sex");

        //表与表关系：1对多，参数一：多属性，参数二：属性类型，参数三：1的主键
        t.relationOneToMany(& Author::blogs, "ListBlog", "author_id");

        //不太理解
        t.fct_0<int>(std::mem_fn(& Author::age), "age");
    }
}

Author::Author() : authorId("0"), sex(UNKOWN)
{

}

Author::~Author()
{

}

int Author::age() const
{
    if (!birthday.isValid()) {
        return -1;
    }
    return (QDate::currentDate().year() - birthday.year());
}
