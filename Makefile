NAME = sensor-ping

# Use realpath if lib is not appearing in ldconfig paths
# -Wl,-rpath /usr/local/lib

all:
	$(CXX) -std=c++1z -fpie -o $(NAME) sensor.cpp ping.cpp -lpthread -ljsoncpp -L /usr/local/lib -lwshook
	# Set capabilities for raw packets
	su -c "setcap cap_net_raw,cap_net_admin=eip $(NAME)"

run:
	su -c "LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:/usr/local/lib ./$(NAME)"

clean:
	rm sensor.o

fclean: clean
	rm $(NAME)

.PHONY: sensor run clean fclean
