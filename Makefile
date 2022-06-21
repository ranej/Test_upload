# C++ compiler
CXX := g++

# Compiler flags
CXXFLAGS := -O2

ARCHOS := x64_rhel7_gcc48

# Dirs where Simmetrix includes and libraries are available
#SIM_DIR :=/opt/scorec/spack/v0154_2/install/linux-rhel7-x86_64/gcc-10.1.0/simmetrix-simmodsuite-16.0-210606dev-j3ay267kiijdajze3kcqxrd2inuwrkbq
SIM_DIR :=/opt/scorec/spack/v0154_2/install/linux-rhel7-x86_64/gcc-10.1.0/simmetrix-simmodsuite-17.0-220516-7botygt2ywcsqoqg454ur3r2zzdzfmlv
#SIM_DIR :=/opt/scorec/spack/v0154_2/install/linux-rhel7-x86_64/gcc-10.1.0/simmetrix-simmodsuite-17.0-220403dev-hxlw2vwqtbuly2pvjcieudrcsu6fuw3r
SIM_INC_DIR := $(SIM_DIR)/include
SIM_LIB_DIR := $(SIM_DIR)/lib/$(ARCHOS)

# Simmetrix includes and libraries
SIM_INC := -I$(SIM_INC_DIR)
SIM_LIB := -L$(SIM_LIB_DIR) -lSimAdvMeshing -lSimMeshing -lSimParasolid330 -lSimDiscrete \
           -lSimMeshTools -lSimModel

# Modeling kernel libraries
MOD_LIB := -Xlinker -rpath -Xlinker $(SIM_LIB_DIR)/psKrnl \
           -L$(SIM_LIB_DIR)/psKrnl -lpskernel

# System libraries
SYS_LIB := -lpthread -lm

#------------------------------------------------------------------------------

obj := obj
bin := bin
setup := $(filter-out $(wildcard $(obj) $(bin)),$(obj) $(bin))

incs := $(SIM_INC)
libs := $(SIM_LIB) $(MOD_LIB) $(SYS_LIB)

objs := $(obj)/reparameterizeForDiscrete.o
exec := $(bin)/reparameterizeForDiscrete

$(exec): $(setup) $(objs)
	$(CXX) $(CXXFLAGS) $(incs) $(objs) $(libs) -o $@

$(obj)/%.o: %.cc
	$(CXX) $(CXXFLAGS) $(incs) -c $< -o $@

obj:
	mkdir $@

bin:
	mkdir $@

clean:
	rm -f $(objs) $(exec)
