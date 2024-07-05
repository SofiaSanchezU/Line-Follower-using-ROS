execute_process(COMMAND "/home/sofy/catkin_ws_2/build/rosserial/rosserial_client/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/sofy/catkin_ws_2/build/rosserial/rosserial_client/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
