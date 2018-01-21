## 事件循环

### 数据类型

#### QUEUE

​	QUEUE是libuv内部大量使用的队列类型，其类型是一个长度为2的数组，数组元素的类型为void*，queue[0]指向队列的前驱节点，queue[1]执行队列的后继节点。

```c
typedef void *QUEUE[2];
```

​	QUEUE_NEXT宏，接收一个指向QUEUE的指针，相当于(QUEUE*)QUEUE[0]，用于获取队列的后继节点。

```c
#define QUEUE_NEXT(q)	(*(QUEUE **) &((*(q))[0]))
```

​	QUEUE_PREV宏，接收一个指向QUEUE的指针，相当于(QUEUE*)QUEUE[1]，用于获取队列的前驱节点。

```c
#define QUEUE_PREV(q)	(*(QUEUE **) &((*(q))[1]))
```

​	QUEUE_INIT宏用于初始化队列，相当于QUEUE[0] = QUEUE[1] = &QUEUE。

```c
#define QUEUE_INIT(q)                                                         \
  do {                                                                        \
    QUEUE_NEXT(q) = (q);                                                      \
    QUEUE_PREV(q) = (q);                                                      \
  }                                                                           \
```

​	QUEUE_FOREACH宏遍历队列中的所有元素

```c
#define QUEUE_FOREACH(q, h)                                                   \
  for ((q) = QUEUE_NEXT(h); (q) != (h); (q) = QUEUE_NEXT(q))
```

​	QUEUE_DATA有QUEUE指针获取到数据节点的指针，传入QUEUE指针，节点类型和QUEUE在节点中的偏移量。

```c
#define QUEUE_DATA(ptr, type, field)                                          \
  ((type *) ((char *) (ptr) - offsetof(type, field)))
```

​	QUEUE_EMPTY宏判断队列是否为空

```c
#define QUEUE_EMPTY(q)                                                        \
  ((const QUEUE *) (q) == (const QUEUE *) QUEUE_NEXT(q))
```

####uv_loop_t

​	事件循环是libuv的核心，在linux下事件循环使用epoll实现，使用数据类型`uv_loop_t`表示，其定义如下：

```1
typedef struct uv_loop_s uv_loop_t;

struct uv_loop_s {
  void* data;
  unsigned int active_handles;
  void* handle_queue[2];
  void* active_reqs[2];
  unsigned int stop_flag;
  UV_LOOP_PRIVATE_FIELDS
};                                                    
```

​	UV_LOOP_RPIVATE_FIELDS宏定义了事件循环的私有变量，展开后的uv_loop_s如下：

```c
struct uv_loop_s {
  void* data;  // 可以用于存储任意数据，uv_loop_t本身不会用到该字段
  unsigned int active_handles;	// 事件循环的引用计数，相当于epoll上感兴趣文件描述符列表大小？
  void* handle_queue[2];
  void* active_reqs[2];
  unsigned int stop_flag;
  unsigned long flags;    
  // UV_LOOP_PRIVATE_FIELDS
  int backend_fd;                                                             
  void* pending_queue[2];                                                      
  void* watcher_queue[2];                                                     
  uv__io_t** watchers;                                                        
  unsigned int nwatchers;  
  unsigned int nfds;                                                           
  void* wq[2];                                                                 
  uv_mutex_t wq_mutex;                                                         
  uv_async_t wq_async;                                                         
  uv_rwlock_t cloexec_lock;                                                    
  uv_handle_t* closing_handles;                                                
  void* process_handles[2];                                                    
  void* prepare_handles[2];                                                    
  void* check_handles[2];                                                      
  void* idle_handles[2];                                                       
  void* async_handles[2];                                                      
  void (*async_unused)(void);
  uv__io_t async_io_watcher;                                                   
  int async_wfd;                                                               
  struct {                                                                     
    void* min;                                                                 
    unsigned int nelts;                                                        
  } timer_heap;                                                                
  uint64_t timer_counter;                                                      
  uint64_t time;                                                               
  int signal_pipefd[2];                                                        
  uv__io_t signal_io_watcher;                                                  
  uv_signal_t child_watcher;                                                   
  int emfile_fd;  
  // UV_PLATFORM_LOOP_FIELDS
  uv_thread_t cf_thread;                                                       
  void* _cf_reserved;                                                          
  void* cf_state;                                                              
  uv_mutex_t cf_mutex;                                                         
  uv_sem_t cf_sem;                                                             
  void* cf_signals[2];
};  
```

#### uv_run_mode

​	事件循环运行模式，可以使用三种模式中的任意一种来运行事件循环，其定义和各自的特点如下：

```c
typedef  enum  { 
    UV_RUN_DEFAULT = 0,
    UV_RUN_ONCE,
    UV_RUN_NOWAIT 
}  uv_run_mode;
```

​	UV_RUN_DEFAULT：一直运行事件循环，直到没有更多的活动和引用句柄或请求。如果uv_stop() 被调用，仍然有活动的句柄或请求则返回非0，在所有其他情况下返回零。

​	UV_RUN_ONCE：执行一次I/O循环,如果没有事件，则阻塞，若没有活动句柄或请求,则返回0，否则返回非0。

​	UV_RUN_NOWAIT：执行一次I/O循环,如果没有事件，不阻塞，若没有活动句柄或请求，则返回0，否则返回非0。

#### uv_walk_cb

​	传递给`uv_walk()`的回调，`uv_walk()`用于遍历事件循环中的所有句柄。

```c
void (*uv_walk_cb)(uv_handle_t *handle，void *arg);

typedef void *QUEUE[2];
void uv_walk(uv_loop_t* loop, uv_walk_cb walk_cb, void* arg) {
  QUEUE queue;
  QUEUE* q;
  uv_handle_t* h;

  QUEUE_MOVE(&loop->handle_queue, &queue);
  while (!QUEUE_EMPTY(&queue)) {
    q = QUEUE_HEAD(&queue);
    h = QUEUE_DATA(q, uv_handle_t, handle_queue);

    QUEUE_REMOVE(q);
    QUEUE_INSERT_TAIL(&loop->handle_queue, q);

    if (h->flags & UV__HANDLE_INTERNAL) continue;
    walk_cb(h, arg);
  }
}
```

### 创建时间循环对象

​	创建uv_loop_t对象会初始化内部的各种数据结构，并调用`epoll_create()`创建一个epoll文件描述符。

​	libuv中可以使用三种方式来创建时间循环类型对象，第一种是动态分配一个对象的内存，并调用`uv_loop_init()`来初始化该对象。

```c
uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
uv_loop_init(loop);
```

​	第一种是直接定义一个uv_loop_t对象，这种情况不需要调用`uv_loop_init()`。

```c
uv_loop_t loop;
```

​	第三中是使用libuv默认的事件循环，该事件循环对象是全局的。

```c
uv_loop_t *loop = uv_default_loop();
```