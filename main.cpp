#include "precompiled.h"
/*
 *     QxOrm_Impl.h，它的作用是检测内存泄露。如果使用 QxMemLeak 模块或 boost::serialization 引擎，
 * 应该在所有的 *.cpp 中包含它；否则，它便是可选的（非必须）
 */
#include "QxOrm_Impl.h"
#include "blog.h"
#include "author.h"
#include "comment.h"
#include "category.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*==================================连接 sqlite 数据库=================================*/
//    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
//    qx::QxSqlDatabase::getSingleton()->setDatabaseName("./Users.db");
//    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
//    qx::QxSqlDatabase::getSingleton()->setUserName("root");
//    qx::QxSqlDatabase::getSingleton()->setPassword("");

    /*==================================连接 MySql 数据库=================================*/
    qx::QxSqlDatabase::getSingleton()->setDriverName("QMYSQL");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName("test");
    qx::QxSqlDatabase::getSingleton()->setHostName("127.0.0.1");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("root");
    qx::QxSqlDatabase::getSingleton()->setPort(3306);
    //在打印日志之前，格式化 sql 语句
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(true);
    //显示执行时间
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(true);
    //只是为了调试：若检测到取得的关系中无效的 offset ，断言中断
    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);

    //创建表
    QSqlError daoError;
    /*==================================创建表（PS：但是对于 mysql 数据库，若主键是 QString，转换成 sql 类型是 text，这个
     * 时候创建表会报错，因为 mysql 主键必须有长度，得改成 varchar(20) 这样的形式，解决办法两个：1.手工创建表；2.更改源码，更改映射类型）
     * =================================*/
//    daoError = qx::dao::create_table<Author>();
//    daoError = qx::dao::create_table<Comment>();
//    daoError = qx::dao::create_table<Category>();
//    daoError = qx::dao::create_table<Blog>();

    /*==================================以集合的形式插入数据库=================================*/
    AuthorPtr author1;
    AuthorPtr author2;
    AuthorPtr author3;
    //这是访问 QSharedPointer 方法，用 .
    author1.reset(new Author());
    author2.reset(new Author());
    author3.reset(new Author());

    //这是访问 Author 属性，用 ->
    author1->authorId = "author_id_1";
    author1->name = "author_1";
    author1->sex = Author::MALE;
    author1->birthday = QDate::currentDate();

    author2->authorId = "author_id_2";
    author2->name = "author_2";
    author2->sex = Author::FEMALE;
    author2->birthday = QDate::currentDate();

    author3->authorId = "author_id_3";
    author3->name = "author_3";
    author3->sex = Author::FEMALE;
    author3->birthday = QDate::currentDate();

    ListAuthor authors;
    authors.insert(author1->authorId, author1);
    authors.insert(author2->authorId, author2);
    authors.insert(author3->authorId, author3);

    //将 Author 集合插入数据库
    daoError = qx::dao::insert(authors);
    //查询表中所有记录总数
    qAssert(qx::dao::count<Author>() == 3);

    /*==================================删除=================================*/
    daoError = qx::dao::delete_by_id(author3);
    qAssert(qx::dao::count<Author>() == 2);

    /*==================================查询=================================*/
    qx::QxSqlQuery query("where author.sex = :sex");
    query.bind(":sex", Author::FEMALE);

    ListAuthor femaleAuthors;
    daoError = qx::dao::fetch_by_query(query, femaleAuthors);
    qAssert(femaleAuthors.count() == 2);

    //转储集合，参数二：是否采用 json 格式，默认采用 xml 格式（就是在控制台显示）
    qx::dump(femaleAuthors, true);
    qx::dump(femaleAuthors, false);

    /*===============测试 qx::QxSqlQuery::freeText() 不带/带 占位符=============*/
    query = qx_query();
    query.freeText("where author.sex = " + QString::number(static_cast<int>(Author::FEMALE)));
    daoError = qx::dao::fetch_by_query(query, femaleAuthors);
    //第二种参数形式
    query = qx_query();
    query.freeText("where author.sex = :sex", QVariantList() << Author::MALE);
    daoError = qx::dao::fetch_by_query(query, femaleAuthors);
    qAssert(femaleAuthors.count() == 1);
    //多个参数形式
    query = qx_query();
    query.freeText("where author.sex = :sex and author.author_id = :authorId",
                   QVariantList() << Author::FEMALE << "author_id_2");
    daoError = qx::dao::fetch_by_query(query, femaleAuthors);
    qAssert(femaleAuthors.count() == 1);

    /*=============================插入，测试数据库事务===========================*/
    CategoryPtr category1 = CategoryPtr(new Category());
    CategoryPtr category2 = CategoryPtr(new Category());
    CategoryPtr category3 = CategoryPtr(new Category());

    category1->name = "category_1";
    category1->desc = "desc_1";
    category2->name = "category_2";
    category2->desc = "desc_2";
    category3->name = "category_3";
    category3->desc = "desc_3";

    {
        //创建一个 scope 来销毁临时的数据库连接

        //打开数据库事务
        QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
        //QxOrm 在连接 mysql 时，默认不打开事务（后面得考虑如何打开事务）
        bool commit = db.transaction();

        daoError = qx::dao::insert(category1, &db);
        commit = (commit && !daoError.isValid());
        daoError = qx::dao::insert(category2, &db);
        commit = (commit && !daoError.isValid());
        daoError = qx::dao::insert(category3, &db);
        commit = (commit && !daoError.isValid());

//        qAssert(commit);
        qAssert(category1->categotyId != 0);
        qAssert(category2->categotyId != 0);
        qAssert(category3->categotyId != 0);

        //决定事物是提交还是回滚
        if (commit) {
            db.commit();
        } else {
            db.rollback();
        }
    } //结束 scope，‘db’ 就被销毁了

    /*=============================用 qx::dao::save() 插入数据===========================*/
    //利用工厂方法 + 类名，创建类
    qx::any blogAny = qx::create("Blog");
    BlogPtr blog1;
    try {
        blog1 = qx::any_cast<BlogPtr>(blogAny);
    } catch (...) {
        blog1.reset(new Blog());
    }

    blog1->text = "blog_text_1";
    blog1->createTime = QDateTime::currentDateTime();
    blog1->author = author1;
    //用 save() 方法插入数据记录
    daoError = qx::dao::save(blog1);

//    blog1->text = "update blog_text_1";
//    blog1->author = author2;
//    //修改属性，并用 save() 方法保存修改（经测试，并不会修改，反而新增了一个纪录）
//    daoError = qx::dao::save(blog1);

    /*=============================单个插入数据===========================*/
    CommentPtr comment1 = CommentPtr(new Comment());
    CommentPtr comment2 = CommentPtr(new Comment());
    comment1->text = "comment_1 text";
    comment1->createTime = QDateTime::currentDateTime();
    comment1->blog = blog1;
    comment2->text = "comment_2 text";
    comment2->createTime = QDateTime::currentDateTime();
    comment2->blog = blog1;

    daoError = qx::dao::insert(comment1);
    daoError = qx::dao::insert(comment2);
    //查询表中记录总数
    qAssert(qx::dao::count<Comment>() == 2);

    /*==================================保存关联数据=================================*/
    //插入多对多关系
    blog1->categories.insert(category1->categotyId, category1);
    blog1->categories.insert(category2->categotyId, category3);
    //参数一：多方属性类型  参数二：插入对象
    daoError = qx::dao::save_with_relation("ListCategory", blog1);

    /*==================================查询关联数据=================================*/
    BlogPtr blogTemp;
    blogTemp.reset(new Blog());
    blogTemp->blogId = 6;
    //根据对象 id 获取该对象所有关联数据
    daoError = qx::dao::fetch_by_id_with_all_relation(blogTemp);

    qAssert(blogTemp->comments.count() == 2);
    qAssert(blogTemp->categories.count() == 2);
    qAssert(blogTemp->text == "blog_text_1");
    qAssert(blogTemp->author && blogTemp->author->authorId == "author_id_1");

    //转储集合，参数二：是否采用 json 格式，默认采用 xml 格式
    qx::dump(blogTemp, false);
    qx::dump(blogTemp, true);

    /*==================================看不太懂=================================*/
    ListBlog blogs;
    daoError = qx::dao::fetch_all_with_relation(QStringList() << "{ blog_text }" << "author_id {name, birthday}"
                                                << "ListComment {comment_text} -> blog_id -> *", blogs);
    qx::dump(blogs, true);

    /*==================================对象与json字符串互相转换=================================*/
#ifndef _QX_NO_JSON
    //将对象转换成 json 字符串
    QString jsonFull = qx::serialization::json::to_string(blogTemp, 1);
    //参数三：过滤条件，只查找条件中的字段
    QString jsonFiltered = qx::serialization::json::to_string(blogTemp, 1,
                                                              "filter: { blog_text } | author_id {name, birthday}"
                                                              "| ListComment {comment_text} -> blog_id -> *");
    qDebug("full : \n%s", qPrintable(jsonFull));
    qDebug("filter : \n%s", qPrintable(jsonFiltered));

    BlogPtr blogFull;
    blogFull.reset(new Blog());
    BlogPtr blogFilter;
    blogFilter.reset(new Blog());

    //将 json 字符串转换成对象
    qx::serialization::json::from_string(blogFull, jsonFull, 1);
    qx::serialization::json::from_string(blogFilter, jsonFull, 1, "filter: { blog_text } | author_id {name, birthday}"
                                                                "| ListComment {comment_text} -> blog_id -> *");
#endif

    /*==================================看不太懂=================================*/
    ListBlog blogs1;
    daoError = qx::dao::fetch_all_with_relation(QStringList() << "<blog_alias> {blog_text}"
                                                << "author_id <author_alias> {name, birthday}"
                                                << "ListComment <ListComment_alias> {comment_text} -> blog_id <blog_alias_2> -> *<..._my_alias_suffix>",
                                                blogs1);
    qx::dump(blogs1);

    /*==================================增加子查询=================================*/
    ListBlog blogs2;
    query = qx_query().where("blog_alias.blog_text").isEqualTo("blog_text_1");
    query.addJoinQuery("ListComment_alias", "and ListComment_alias.comment_text is not null");
    query.addJoinQuery("author_alias", qx_query().freeText("and author_alias.sex = :sex", QVariantList() << Author::FEMALE));
    daoError = qx::dao::fetch_by_query_with_relation(QStringList() << "<blog_alias> {blog_text}"
                                                     << "author_id <author_alias> {name, birthday, sex}"
                                                     << "ListComment <ListComment_alias> {comment_text}",
                                                     query, blogs2);
    qx::dump(blogs2);

    /*==================================循环保存，我的理解是把所有相关联的数据都保存到数据库中=================================*/
    /*==================================看不出来有啥用=================================*/
    daoError = qx::dao::save_with_relation_recursive(blogTemp);
    qAssert(!daoError.isValid());
    daoError = qx::dao::save_with_relation_recursive(blogTemp, qx::dao::save_mode::e_update_only);
    qAssert(!daoError.isValid());

    /*==================================通过类名+方法名，调用 ‘age()’ 反射=================================*/
    qx_bool localInvoke = qx::QxClassX::invoke("Author", "age", author1);
    qAssert(localInvoke);

    /*==================================更新指定属性=================================*/
    ListBlog blogs3;
    //查询所有 blog 表数据
    daoError = qx::dao::fetch_all(blogs3);
    qAssert(blogs3.count() == 1);
    BlogPtr blog5 = blogs3.at(0);
    qAssert(blog5->text == "blog_text_1");
    //包装，因为 qx::dao::update_optimized() 只识别这样的参数类型：qx::dao::ptr<Blog>
    qx::dao::ptr<Blog> blog6 = qx::dao::ptr<Blog>(blog5);
    blog6->text = "update blog_text_1";
    //更新指定属性
    daoError = qx::dao::update_optimized(blog6);
    qAssert(!daoError.isValid());

    /*==================================只取指定列数据=================================*/
    ListBlog blogs4;
    //只取 “date_creation” 列数据
    qx::dao::fetch_all(blogs4, NULL, QStringList() << "date_creation");
    qx::dump(blogs4);

    /*==================================显示所有注册到 QxOrm 上下文中的类=================================*/
    qx::QxClassX::dumpAllClasses();

    /*==================================执行自定义 sql 语句/存储过程=================================*/
    qx_query customSql("select * from author");
    daoError = qx::dao::call_query(customSql);
    customSql.dumpSqlResult();

    /*==================================执行自定义 sql 语句/存储过程，并将结果放入集合中=================================*/
    ListAuthor authors1;
    daoError = qx::dao::execute_query(customSql, authors1);
    qx::dump(authors1);
    /*==================================执行自定义 sql 语句/存储过程，并取结果集合中的第一个返回=================================*/
    qx_query customSqlCategory("select name, category_id from category");
    CategoryPtr category6 = CategoryPtr(new Category());
    daoError = qx::dao::execute_query(customSqlCategory, category6);
    qx::dump(category6);



    return a.exec();
}
