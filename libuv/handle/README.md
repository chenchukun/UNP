## handle

​	uv_handle_t是libuv中所有句柄类型的基类，可以看做是对于文件描述符的封装，其定义如下：

```c
typedef struct uv_handle_s uv_handle_t;

struct uv_handle_s {
//    UV_HANDLE_FIELDS
	void* data;          	// 可用于存储用户数据                                                  
  	uv_loop_t* loop;        // handle所在的事件循环                                  
  	uv_handle_type type;    // 句柄类型                                                    
  	                                                        
  	uv_close_cb close_cb; 	// 关闭句柄的回调                                                      
  	void* handle_queue[2]; 	// 队列节点，用于挂载到event_loop的handle_queue队列上                                                     
  	union {                 // 句柄实际应用的资源，对于文件、套接字等为描述符                                                    
    	int fd;                                                                   
    	void* reserved[4];                                                        
  	} u;                                                                        
//  	UV_HANDLE_PRIVATE_FIELDS 
  	uv_handle_t* next_closing;   // 当handle关闭时，会挂载到event_loop的closing_handles上	                                                
  	unsigned int flags; 
};
```

