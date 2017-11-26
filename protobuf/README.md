# Protocol buffers

## protobuf简介

### 什么是protobuf

​	Protocol buffers是谷歌实现的一种跨平台，支持多语言，可扩展的数据交换格式，用于对结构化数据进行序列化，它比JSON、XML格式更小、更快、更简单。只需要将被序列化的数据结构定义一次，就可以通过工具自动生成多种语言的代码，轻松的使用不同语言来实现对这一数据结构进行读写和序列化、反序列化操作。

### protobuf的使用流程

#### 一、编写.proto

​	在使用protobuf前，需要使用protobuf提供的语法定义数据结构，语法类似于C++，每一条消息由多个键值对组成。每个消息类型都有一个或多个唯一编号的字段。每个字段都有一个名称和一个值类型，其中值类型可以是数字（整数或浮点数），布尔值，字符串，原始字节，甚至（如上例所示）其他消息类型，允许嵌套的定义消息结构。每个字段都可以指定可选字段，必填字段或重复字段。

```protobuf
message Person {
  required string name = 1;
  required int32 id = 2;
  optional string email = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    required string number = 1;
    optional PhoneType type = 2 [default = HOME];
  }

  repeated PhoneNumber phone = 4;
}
```

#### 二、编译.proto文件

​	消息的定义独立于任何的编程语言，定义好消息后，就可以使用probobuf提供的编译器来生成数据访问类，一个消息对应生成一个类，类的每一个字段提供了setter、getter等方法来读写该字段，且提供了对消息进行序列化和反序列化的方法。

​	比如若编译生成的为C++代码，这可以这样使用定义的消息。

```protobuf
// 定义Person对象，并设置值，然后序列化写入文件myfile中
Person person;
person.set_name("John Doe");
person.set_id(1234);
person.set_email("jdoe@example.com");
fstream output("myfile", ios::out | ios::binary);
person.SerializeToOstream(&output);

// 从myfile中读取序列化的结果，并反序列化为Person对象。
fstream input("myfile", ios::in | ios::binary);
Person person;
person.ParseFromIstream(&input);
cout << "Name: " << person.name() << endl;
cout << "E-mail: " << person.email() << endl;
```

### 与XML的比较

​	与XML相比，protobuf又以下优点：

* 比较简单

* 比XML小3到10倍

* 比XML快20到100倍

* 没有歧义

* 生成更容易以编程方式使用的数据访问类

* 可扩展，可以随时添加新字段，旧的代码不需要知道新字段的存在就可以正常的解析新消息。

  缺点：

* 数据对于人类不可读

### protobuf的关键字和数据类型

​	protobuf支持一下数据类型，每一种类型都可以与C++的内置类型相对应。

| protobuf 数据类型 | 描述                     | 序列化后大小    | C++语言映射          |
| ------------- | ---------------------- | --------- | ---------------- |
| bool          | 布尔类型                   | 1字节       | bool             |
| double        | 64位浮点数                 | N         | double           |
| float         | 32为浮点数                 | N         | float            |
| int32         | 32位整数、                 | N         | int              |
| uin32         | 无符号32位整数               | N         | unsigned int     |
| int64         | 64位整数                  | N         | int64_t          |
| uint64        | 64为无符号整                | N         | uint64_t         |
| sint32        | 32位整数，处理负数效率更高         | N         | int32_t          |
| sing64        | 64位整数 处理负数效率更高         | N         | int64_t          |
| fixed32       | 32位无符号整数               | 4         | unsigned int32   |
| fixed64       | 64位无符号整数               | 8         | unsigned int64_t |
| sfixed32      | 32位整数、能以更高的效率处理负数      | 4         | unsigned int32_t |
| sfixed64      | 64为整数                  | 8         | unsigned int64_t |
| string        | 只能处理 ASCII字符           | N         | std::string      |
| bytes         | 用于处理多字节的语言字符、如中文       | N         | std::string      |
| enum          | 可以包含一个用户自定义的枚举类型uint32 | N(uint32) | enum             |
| message       | 可以包含一个用户自定义的消息类型       | N         | object of class  |

​	当使用protobuf定义某一种类型的字段时，可以添加一个关键字来修饰该字段，包括以下几个关键字：

- required：必选字段，表示该值是必须要设置的。
- optional：可选字段的，消息格式中该字段可以有0个或1个值（不超过1个），可以包含默认值。
- repeated：重复字段，在一个格式良好的消息中，这种字段可以重复任意多次（包括0次）。重复的值的顺序会被保留。表示该值可以重复，相当于java中的List。

## proto2语言指南

### 定义消息类型

​	proto2的语法类似于C/C++的语法，使用message关键字来定义消息类型，每一个消息类型包含多个字段,比如下面定义了消息类型SearchRequest，它包含三个字段。

```protobuf
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
}
```

### 指定消息类型

​	消息中的每一个字段都有自己的类型，比如SearchRequest中的query为string类型，page_number和result_per_page为int32类型，proto中常用的数据类型，已经与之对应的C++数据类型如下表。

| protobuf 数据类型 | 描述                     | 序列化后大小    | C++语言映射          |
| ------------- | ---------------------- | --------- | ---------------- |
| bool          | 布尔类型                   | 1字节       | bool             |
| double        | 64位浮点数                 | N         | double           |
| float         | 32为浮点数                 | N         | float            |
| int32         | 32位整数、                 | N         | int              |
| uin32         | 无符号32位整数               | N         | unsigned int     |
| int64         | 64位整数                  | N         | int64_t          |
| uint64        | 64为无符号整                | N         | uint64_t         |
| sint32        | 32位整数，处理负数效率更高         | N         | int32_t          |
| sing64        | 64位整数 处理负数效率更高         | N         | int64_t          |
| fixed32       | 32位无符号整数               | 4         | unsigned int32   |
| fixed64       | 64位无符号整数               | 8         | unsigned int64_t |
| sfixed32      | 32位整数、能以更高的效率处理负数      | 4         | unsigned int32_t |
| sfixed64      | 64为整数                  | 8         | unsigned int64_t |
| string        | 只能处理 ASCII字符           | N         | std::string      |
| bytes         | 用于处理多字节的语言字符、如中文       | N         | std::string      |
| enum          | 可以包含一个用户自定义的枚举类型uint32 | N(uint32) | enum             |
| message       | 可以包含一个用户自定义的消息类型       | N         | object of class  |

### 分配标签

​	我们需要给消息中的每一个字段分配一个唯一的编号标签，这些标签用于标识消息的二进制格式的字段，一旦消息类型正在使用，就不应该更改，否则会造成无法兼容的问题。可以使用的最小标签为1，最大标签为2^29-1，但不能使用数字19000到19999自己的编号，这些编号被保留用于protobuf的实现使用。编号1到15在二进制格式中需要一个字节进行编码，16到2047的编号需要用两个字节编码，为了节省空间，频繁使用的字段可使用1到15的编号，为了以后可能添加新的频繁使用的字段，可以预留部分1到15的编号待以后使用。

### 指定字段规则

​	消息中的每个字段都可以指定一个规则，规则包括：

- required：必选字段，即消息中必须包含的字段，应该尽量避免使用必选参数。

- optional：可选字段的，消息格式中该字段可以有0个或1个值（不超过1个），可以包含默认值。

- repeated：重复字段，在一个格式良好的消息中，这种字段可以重复任意多次（包括0次），相当于列表。

  ​由于历史原因，`repeated`整数字段类型没有被很好的编码，因此我们应该在代码中使用特殊选项`[packed=true]`来获得更高效的编码。例如:

```protobuf
repeated int32 samples = 4 [packed=true];
```

### 添加更多的消息类型

多个消息类型可以在单个`.proto`文件中定义。如要添加一个用于返回搜索结构的消息类型，可以这么写。

```protobuf
message SearchRequest {
required string query = 1;
optional int32 page_number = 2;
optional int32 result_per_page = 3;
}

message SearchResponse {
...
}
```

### 注释

​	proto使用C/C++风格的注释，可以使用// 进行单行注释，使用/* … */进行多行注释。

### 保留字段

当你在某次更新消息中屏蔽或者删除了一个字段的话，未来的使用着可能在他们的更新中重用这个标签数字来标记他们自己的字段。然后当他们加载旧的消息的时候就会出现很多问题，包括数据冲突，隐藏的bug等等。指定这个字段的标签数字（或者名字，名字可能在序列化为JSON的时候可能冲突）标记为`reserved`来保证他们不会再次被使用。如果以后的人试用的话protobuf编译器会提示出错。

```protobuf
message Foo {
	reserved 2, 15, 9 to 11;
	reserved "foo", "bar";
}
```

### .proto文件生成目标代码

​	当你使用`protoc` 来编译一`.proto`文件的时候，编译器将利用你在文件中定义的类型生成你打算使用的语言的代码文件。生成的代码包括`getting setting` 接口和序列化，反序列化接口。

- 对于C++，编译器对每个`.proto`文件生成一个`.h`和一个`.cc`文件。 每个消息生成一个class。
- 对于Java ， 编译器为每个消息生成一个`.java`文件，外加一个特殊的`Builder`类来生成消息实例。
- 对于Python ， 一点点不同 —– Python编译器生成有一个静态的对每个消息的描述器的模块。然后，用一个元类在运行时创建必要的Python数据访问类。
- 对于Go ， 编译器对文件中的每个消息生成一个`.pb.go`文件。

### 标量数据类型

​	标量数据类型包含以下几种，该表显示了proto文件中指定的类型与其他常用语言对于生成的类型之间的映射关系。

| proto    | Note                                 | C++      | Java       | Python      | Go       |
| -------- | ------------------------------------ | -------- | ---------- | ----------- | -------- |
| float    |                                      | float    | float      | float       | *float32 |
| double   |                                      | double   | double     | float       | *float64 |
| int32    | 变长编码. 编码负数效率底下– 打算使用负数的话请使用 sint32.  | int32_t  | int        | int         | *int32   |
| int64    | 变长编码. 编码负数效率底下– 打算使用负数的话请使用 sint64.  | int64_t  | long       | int/long    | *int64   |
| uint32   | 变长编码.                                | uint32_t | int        | int/long    | *uint32  |
| uint64   | 变长编码.                                | uint64_t | long       | int/long    | *uint64  |
| sint32   | U变长编码. 数值有符号，负数编码效率高于int32           | int32_t  | int        | int         | *int32   |
| sint64   | U变长编码. 数值有符号，负数编码效率高于int64           | int64_t  | long       | int/long    | *int64   |
| fixed32  | 固定4byte， 如果数值经常大于2的28次方的话效率高于uint32. | uint32_t | int        | int         | *uint32  |
| fixed64  | 固定8byte， 如果数值经常大于2的56次方的话效率高于uint64. | uint64_t | long       | int/long    | *uint64  |
| sfixed32 | 固定4byte.                             | int32_t  | int        | int         | *int32   |
| sfixed64 | 固定8byte.                             | int64_t  | long       | int/long    | *int64   |
| bool     |                                      | bool     | boolean    | bool        | *bool    |
| string   | 字符串内容应该是 UTF-8 编码或者7-bit ASCII 文本.   | string   | String     | str/unicode | *string  |
| bytes    | 任意二进制数据.                             | string   | ByteString | str         | []byte   |

### 可选字段和默认值

​	被标记为`optional`的字段，在最终的消息中，可能包含也可能不包含该字段，当消息被解析时，若它不包含可选字段，则解析对象中相应字段被设置为该字段的默认值。默认值可以在定义消息时指定。例如：

```protobuf
optional int32 result_per_page = 3 [default = 10];
```

​	若定义消息时为提供默认值，则将对该类型执行默认初始化，如对于字符串，将是一个空串。对于数值类型，将是0.对于枚举类型，将是枚举类型列出的第一个值。

### 枚举

​	proto支持枚举类型，枚举常数必须是一个32为的整数。由于枚举值在通讯的时候使用变长编码，所以负数的效率很低，不推荐使用。你可以在（像上面这样）在一个消息内定义枚举，也可以在消息外定义 —– 这样枚举就在全文件可见了。如果你想要使用在消息内定义的枚举的话，使用语法 `MessageType.EnumType`。 其定义如下：

```protobuf
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3 [default = 10];
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  optional Corpus corpus = 4 [default = UNIVERSAL];
}
```

### 使用其他的消息类型

​	在定义一个消息类型时，可以使用其他消息类型作为字段类型，比如在SearchResponse消息中包含Result类型的消息。

```protobuf
message SearchResponse {
  repeated Result result = 1;
}

message Result {
  required string url = 1;
  optional string title = 2;
  repeated string snippets = 3;
}
```

### 导入定义

​	类似于C/C++，可以使用import语句来导入在其他文件中定义的消息类型，protoc的默认搜索路径为当前目录，可以通过-I/--proto_path 来指定搜索路径。

```protobuf
import "myproject/other_protos.proto"; 
```

### 内嵌类型

​	你可以在一个消息中定义并使用其他消息类型，比如下面的例子 —— `Result`消息是在`SearchResponse`中定义的 ：

```protobuf
message SearchResponse {
  message Result {
    required string url = 1;
    optional string title = 2;
    repeated string snippets = 3;
  }
  repeated Result result = 1;
}12345678
```

​	如果你打算在这个消息的父消息之外重用这个消息的话，你可以这样引用它 ： `Parent.Type`

```protobuf
message SomeOtherMessage {
  optional SearchResponse.Result result = 1;
}123
```

​	你想嵌套多深就嵌套多深，没有限制 ：

```protobuf
message Outer {                  // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      required int64 ival = 1;
      optional bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      required int32 ival = 1;
      optional bool  booly = 2;
    }
  }
}
```

### 更新一个消息

​	如果一个现有的消息类型不再满足你的需求，比如你需要额外的字段，但是你仍然希望兼容旧代码生成的消息的话，不要担心！ 在不破坏现有代码的前提下更新消息是很简单的。请铭记下面的规则 ：

- 不要改变任何已有的数字标签
- 你新添加的字段需要是`optional`或者`repeated`。由于任何`required`字段都没有丢失，这意味着你的旧代码序列化的消息能够被新代码解析通过。你应该给新的字段设置合理的默认值，这样新的代码可以合适解析使用旧的消息。同样的，新的代码产生的消息包也可以被旧的代码解析通过，旧的代码在解析的时候会忽略新的字段。不过新的字段并没有被丢弃,如果这个消息在旧的代码中再次被序列化，这些未知的字段还会在里面 —— 这样这些消息被传递回新的代码的时候，解析仍然有效。
- 非`required`字段可以被移除，但是对应的数字标签不能被重用。或许你可以通过重命名这个字段，加上前缀`OBSOLETE_`来表示废弃。或者你可以标记`reserverd`。这样你未来就不会不小心重用这些字段了。
- 只要保证标签数字一致，一个非`required`字段可以被转化扩展字段，反之亦然。
- `int32, uint32, int64, uint64, 和 bool`这些类型是兼容的 —— 这意味着你可以将一个字段的类型从其中的一种转化为另一种，不会打破向前向后兼容！ 如果通信的时候传输的数字不符合对应类型的那么你会得到和C++中强制类型转化一样的效果（64bit数字会被截断）。
- `sint32 sint64`相互兼容，但是不和其他的数字类型兼容。
- `string bytes`相互兼容 ，前提是二进制内容是有效的UTF-8 。
- `optional repeated`是兼容的。当给定的输入字段是`repeated`的时候，如果接收方期待的是一个`optional`的字段的话，对与原始类型的字段，他会取最后一个值，对于消息类型的字段，他会将所有的输入合并起来。
- 你可以改变一个默认初始值，反正这个初始值从来不再通讯中传递。因此， 如果一个字段没有被设置，那么解析程序就将它赋值为解析程序所使用的版本的默认初始值，而不是发送方的默认初始值。
- 枚举类型和`int32, uint32, int64, and uint64`在传输格式中相互兼容（注意如果不合适会被 截断），但是接收方在发序列化的时候处理他们可不大一样。请注意： 反序列化的时候不正确的枚举数字会被丢弃，这样这个字段的`has_xxx`接口就返回`false`并且`get_xxx`接口返回枚举的第一个值。不过如果是一个整形字段的话，这个数值会一致保留。所以当你打算把一个整形更新为枚举的时候，请务必注意整数的值不要超出接收方枚举的值。

### 扩展 extemsions

​	`extensions` 让你定义一段可用的数字标签来供第三方扩展你的消息。其他人可以在他们自己的文件里面使用这些标签数字来扩展你的下消息（无需修改你的消息文件）。 举个例子：

```protobuf
message Foo {
  //，，，
  extensions 100 to 199;
}1234
```

这意味着`Foo` 消息在[ 100 , 199 ]区间的标签数字被保留做扩展使用。其他的使用者可以在他们自己的文件中导入你的文件，然后在他们自己的文件中给你的消息添加新的字段 ：

```protobuf
extend Foo {
  optional int32 bar = 126;
}123
```

这样就意味着`Foo`消息现在有一个叫做`bar`的`int32`字段了。在编码的时候，通讯格式和使用者定义的新的消息一样。不过你的程序访问扩展字段的方式和访问常规字段的方式不太一样， 这里以C++代码为例 ：

```protobuf
Foo foo;
foo.SetExtension(bar, 15);12
```

类似的，`Foo`类有以下接口`HasExtension(), ClearExtension(), GetExtension(), MutableExtension(), and AddExtension()` 。 
注意扩展字段可以是除了`oneof`或者`map`外的其他任何类型，包括消息类型。

### Oneof

​	Oneof类似于union，如果在消息中有多个可选字段，且同一时刻只有其中一个字段被设置，可以使用oneof来强化这个特性，这样可以更加节省存储空间。

oneof字段类似optional字段只不过oneof里面所有的字段共享内存，而且统一时刻只有一个字段可以被设着。设置其中任意一个字段都自动清理其他字段。在你的代码中，你可以使用case()或者 WhichOneOf()接口来查看到底是哪个字段被设置了。

```protobuf
message SampleMessage {
  oneof test_oneof {
     string name = 4;
     SubMessage sub_message = 9;
  }
}
```

### Maps

如果你打算在你的数据结构中创建一个关联表的话，我们提供了很方便的语法：

```protobuf
map<key_type, value_type> map_field = N;1
```

这里key_type可以是任意整形或者字符串。而value_tpye 可以是任意类型。 
举个例子，如果你打算创建一个Project表，每个Project关联到一个字符串上的话 ：

```protobuf
map<string, Project> projects = 3;1
```

现在生成Map的API对于所有支持proto2的语言都可用了。

在通讯中，map等价与下面的定义， 这样不支持Map的版本也可以解析你的消息：

```protobuf
message MapFieldEntry {
  key_type key = 1;
  value_type value = 2;
}

repeated MapFieldEntry map_field = N;
```

### Packages概念

为了防止不同消息之间的命名冲突，你可以对特定的`.proto`文件提指定packet 名字 。

```
package foo.bar;
message Open { ... }12
```

在定义你的消息字段类型的时候你可以指定包的名字：

```
message Foo {
  ...
  required foo.bar.Open open = 1;
  ...
}12345
```

包名字的实现取决于你工作的具体编程语言：

- 在C++中 ，生成的消息被包被在一个包名字的命名空间中，比如上面的代码中Bar类是 ： foo::bar。
- 在 Java中,除非你指定了选项`java_package`，否则这个包名字就是Java的包名字。
- 在 Python中,由于Python的模块是由它的文件系统来管理的，所以包名被忽略。

### 包和名字解析

protobuf的名字解析方式和C++很像。首先是最里面的作用域被搜索，然后是外面的一层。。。 没一个包都从他自己到它的父辈。但是如果前面有`.`号的话就（比如`foo.bar.Baz`）意味着从最外面开始。

protobuf 编译器通过所有导入`.proto`文件来解析所有的名字。代码生成器为每个语言生成对应的合适的类型。

### 定义服务 （ Services ）

如果打算将你的消息配合一个RPC(Remote Procedure Call 远程调用）系统联合使用的话，你可以在`.proto`文件中定义一个RPC 服务接口然后protobuf就会给你生成一个服务接口和其他必要代码。比如你打算定义一个远程调用，接收SearchRequest返回SearchResponse， 那么你在你的文件中这样定义 ：

```protobuf
service SearchService {
  rpc Search (SearchRequest) returns (SearchResponse);
}123
```

默认情况下，编译器给你生成一个纯虚接口名叫`SearchRequest`和一个对应的桩实现。这个桩实现直接调用RpcChannel，这个是你自己实现的具体RPC代码。比如你打算实现一个RpcChannel来序列化消息并且使用HTTP发送。换句话说，生成的代码提供了一个基于你的RPC的类型的安全的协议接口实现，它 不需要知晓你的PRC 的任何实现细节。因此最后你的代码大体是这样的 ：

```protobuf
using google::protobuf;

protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
SearchService* service;
SearchRequest request;
SearchResponse response;

void DoSearch() {
  // 你自己提供MyRpcChannel和MyRpcController两个类，这两个类分别实现了纯虚接口
  // s protobuf::RpcChannel 和protobuf::RpcController.
  channel = new MyRpcChannel("somehost.example.com:1234");
  controller = new MyRpcController;
  service = new SearchService::Stub(channel);

  // Set up the request.
  request.set_query("protocol buffers");

  // Execute the RPC.
  service->Search(controller, request, response, protobuf::NewCallback(&Done));
}

void Done() {
  delete service;
  delete channel;
  delete controller;
}123456789101112131415161718192021222324252627
```

所有的服务器类同样实现服务接口。这提供了一种在不知道方法名字和参数的情况下调用方法的途径。在服务器这边，你需要实现一个可以注册服务的PRC服务器。

```protobuf
using google::protobuf;

class ExampleSearchService : public SearchService {
 public:
  void Search(protobuf::RpcController* controller,
              const SearchRequest* request,
              SearchResponse* response,
              protobuf::Closure* done) {
    if (request->query() == "google") {
      response->add_result()->set_url("http://www.google.com");
    } else if (request->query() == "protocol buffers") {
      response->add_result()->set_url("http://protobuf.googlecode.com");
    }
    done->Run();
  }
};

int main() {
//你自己提供的MyRpcServer类，它不需要实现任何接口，这里意思意思就行。
  MyRpcServer server;

  protobuf::Service* service = new ExampleSearchService;
  server.ExportOnPort(1234, service);
  server.Run();

  delete service;
  return 0;
}12345678910111213141516171819202122232425262728
```

如果你不想嵌入你自己的已经存在的RPC系统，你现在可以使用gRPC ： 这是一种谷歌开发的语言和平台无关的开源RPC系统。gPRC和protobuf配合的格外方便。在添加了特定的插件后，它可以从你的`.proto`文件直接生成对应的RPC代码。不过由于proto2和proto3之间存在兼容问题，我们推荐你使用proto3来定义你的gPRC服务。如果你打算使用gPRC配合protobuf ， 你需要3.0.0以上的版本。

### 生成你的代码

如果你要用`.proto`文件生成 C++ , Java, Python的代码的话，你需要使用protoc来编译`.proto`文件。如果你还没安装这个编译器的话，去下载一个吧。

如下执行协议的编译:

> protoc –proto_path=IMPORT_PATH –cpp_out=DST_DIR –java_out=DST_DIR –python_out=DST_DIR path/to/file.proto

- IMPORT_PATH 指定查找`.proto`文件的搜索目录，默认是当前的工作目录。可以多次使用这个参数来指定多个目录，他们会按照顺序被检索， `-I=IMPORT_PATH` 是 `--proto_path`的简写。
- 你可以指定特定的输出路径:
  - `--cpp_out` C++ code in DST_DIR.
  - `--java_out` generates Java code in DST_DIR.
  - `--python_out` generates Python code in DST_DIR.

作为一个额外的便利，如果`DST_DIR`以`.zip` 或者`.jar` 来结尾的话，编译器会自动给你打包。注意如果指定路径已经存在的话会被覆盖。

- 你必须提供一个或多个`.proto`文件。多个文件可以一次全给定。文件名必须是相对当前目录的相对路径名。每个文件都应该在`IMPORT_PATHs` 指定的某个路径下！

## 反射

​	protobuf通过Descriptor获取任意message或service的属性和方法，Descriptor主要包括了以下集中类型：

* FileDescriptor 　　获取proto文件中的Descriptor和ServiceDescriptor
* Descriptor 　　　  获取类message属性和方法，包括FieldDescriptor 和 EnumDescriptor等
* FieldDescriptor 　 获取message中各个字段的类型、标签、名称等
* EnumDescriptor   获取Enum中的各个字段名称、值等
* ServiceDescriptor 获取service中的MethodDescriptor
* MethodDescriptor 获取各个rpc中的request、response、名称等

​	当我们获得FileDescriptor后，就相当于获得了.proto文件的所有信息，进而可以获得其他的Descriptor。在使用protoc编译.proto文件后，protoc已将所有的Descriptor放在DescriptorPool中，可以根据proto的文件名，通过DescriptorPool获取到相应的FileDescriptor。也可以直接根据message或service的名称从DescriptorPool中获得相应的Descriptor。

