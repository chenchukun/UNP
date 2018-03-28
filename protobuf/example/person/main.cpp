#include "person.pb.h"
#include <iostream>
#include <string>
using namespace std;

string serialize()
{
    Person person;
    person.set_name("kikuchanj");
    person.set_id(6180);
    person.set_email("kikuchanj@kikuchanj.cn");
    Person_PhoneNumber *person_phoneNumber = person.add_phone();
    person_phoneNumber->set_number("6180-6180-6180");
    person_phoneNumber->set_type(Person::PhoneType::Person_PhoneType_HOME);
    string outStr;
    person.SerializeToString(&outStr);
    return outStr;
}

Person unserialize(const string &out)
{
    Person person;
    person.ParseFromString(out);
    return person;
}


int main()
{
	string out = serialize();
    cout << "========序列化结果========" << endl;
    cout << out << endl << endl;
    Person person = unserialize(out);

    // 返回一个可读的消息字符串,用于调试
    cout << "========反序列化结果========" << endl;
    cout << person.DebugString() << endl;

    // 返回解析此消息时遇到的未知字段集合。
    cout << "========未知字段集合========" << endl;
    const auto &fields = person.unknown_fields();
    cout << fields.field_count() << endl;

    // static const Descriptor* descriptor()
    // 返回类型的描述符。这包含有关该类型的信息，包括它具有的字段以及它们的类型。这可以用反射来检查字段编程。
    cout << "========类型描述========" << endl;
    const google::protobuf::Descriptor *desc = person.descriptor();
    cout << "name(): " << desc->name() << endl;
    cout << "full_name(): " << desc->full_name() << endl;

    // static const Foo& default_instance()
    // 返回一个常量单实例,所有字段都未设置
    const Person &cperson = person.default_instance();

    // 嵌套定义的类型,将生产两个类,可以使用两种方法访问定义在内部的类。
    Person_PhoneNumber person_phoneNumber;
    // Person 使用typedef Person_PhoneNumber PhoneNumber
    Person::PhoneNumber phoneNumber;
    cout << "========字段操作========" << endl;
    // 判断字段是否被设置
    cout << "has_number(): " << phoneNumber.has_number() << endl;
    phoneNumber.set_number("0663");
    phoneNumber.set_type(Person::HOME);
    cout << "has_number(): " << phoneNumber.has_number() << endl;
    cout << phoneNumber.DebugString() << endl;
    // 获取字段引用,若指定未被设置将返回空串(指定类型的默认值)。
    const string &number = phoneNumber.number();
    // 清楚字段
    phoneNumber.clear_number();
    cout << "has_number(): " << phoneNumber.has_number() << endl;
    // 这种初始化方式接收一个C风格字符串,并可以指定长度,而不是通过检测0字符来确定长度,是二进制安全的
    phoneNumber.set_number("0663", 4);
    phoneNumber.clear_number();
    cout << "has_number(): " << phoneNumber.has_number() << endl;
    // 返回一个可变对象的指针,调用该方法后,指定的值将被设置为空
    // 对返回的指针修改后会改变该字段的值
    string *pnumber = phoneNumber.mutable_number();
    *pnumber = "0618";
    cout << "has_number(): " << phoneNumber.has_number() << endl;
    cout << "phoneNumber.number(): " << phoneNumber.number() << endl;
    // set_allocated_*接收一个指针,若指针为空,相当于clear_*,
    // 若不为空,将接收这个指针的所有权,has_*返回true
    // 指针必须是动态分配的,且调用set_allocated_*后我们不能释放该指针,指针的所有权由对象所有,声明周期由对象管理
    string *al = new string("0663");
    phoneNumber.set_allocated_number(al);
    cout << "*al: " << *al << endl;
    cout << phoneNumber.number() << endl;
    // delete al;  不能delete该对象,反正将造成重复delete
    // 释放字段所有权,重新获得字段的所有权。
    al = phoneNumber.release_number();
    cout << *al << endl;
    cout << phoneNumber.has_number() << endl;
    delete al;     // 必须delete,否则会造成内存泄露

    // 重复字段
    cout << "========重复字段操作========" << endl;
    // 返回重复字段中元素的个数
    cout << person.phone_size() << endl;
    cout << person.newelem_size() << endl;
    // 追加新元素
    person.add_newelem(10);
    person.add_newelem(20);
    // 设置索引处元素的值为新值
    person.set_newelem(1, 30);
    for (int i=0; i<person.newelem_size(); ++i) {
        // 获取值
        cout << person.newelem(i) << ", ";
    }
    cout << endl;
    // 返回RepeatedField存储字段元素的底层
    const google::protobuf::RepeatedField<int32_t> &repeatedField = person.newelem();
    // 返回指向元素的指针,类似于数组
    const int32_t *data = repeatedField.data();
    cout << *data << ", " << *(data+1) << endl;
    cout << repeatedField.Get(0) << endl;
    google::protobuf::RepeatedField<int32_t> *mutableNewelem =  person.mutable_newelem();
    int32_t *mutableData = mutableNewelem->mutable_data();
    *mutableData = 20;
    cout << mutableNewelem->Get(0) << endl;
    // 清空元素
    person.clear_newelem();
    google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
