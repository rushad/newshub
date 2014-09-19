OBJS = client_queue_thread_loop.o server_thread_loop.o socket.o socket_thread_loop.o tcp_client.o \
	tcp_server.o tcp_socket.o thread_loop.o udp_client.o udp_server.o udp_socket.o

%.o : %.cpp
	g++ -c -o $@ $^

libnewshub.a: $(OBJS)
	ar crf libnewshub.a $(OBJS)
