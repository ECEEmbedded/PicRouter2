puts "Building drivers..."

#Write a line to file buffer
def out text
  @fileBuffer << text << "\n"
end

#Get all drivers
driverFiles = []
driverPath = "#{Dir.pwd}/../src/Drivers/"
Dir.entries(driverPath).each do |file|
  if file.include? ".rb" and !file.include? ".sw"
    driverFiles << {
      :name => file.gsub(/\..*/, ""),
      :fullPath => driverPath + file,
    }
  end
end

driverHeaderFileNames = []

#Compile each driver
driverFiles.each do |driver|
  @name = "#{driver[:name]}"
  @fileName = "#{driver[:name]}.h"
  driverHeaderFileNames << @fileName
  puts ""
  puts "##################################"
  driverHeaderFileName = "#{Dir.pwd}/../src/#{@fileName}"
  print "Compiling #{@name}"
  print "#{driverHeaderFileName}"
  puts ""

  @fileBuffer = ""

  #Compile driver###########
  
  #Header
  out "#ifndef __#{@name}_H_"
  out "#define __#{@name}_H_"

  load driver[:fullPath]

  #Helpers##########
  putSelf = lambda do
    out "Driver#{@name}Members *self = (Driver#{@name}Members *)driver;"
  end
  ##################

  #Members##########
  out %{
    typedef struct DRIVER#{@name}MEMBERS \{
        unsigned char id;
        unsigned char class;
  }

    @members.call
  out "} Driver#{@name}Members;"
  ###################

  #Init##########
  out "void Driver#{@name}Init(Driver_t *driver) {"
    putSelf.call
    @init.call
  out "}"
  ###################

  #Respond##########
  out "void Driver#{@name}Respond(Driver_t *driver, unsigned char *rcvData) {"
    putSelf.call
    @respond.call
  out "}"
  ##################

  #Poll##########
  out "void Driver#{@name}Poll(Driver_t *driver) {"
    putSelf.call
    @poll.call
  out "}"
  ##################

  #AddDevice
  out %{
    void Driver#{@name}Add(unsigned char id) \{
      Driver_t *context = (Driver_t *)driverMalloc(sizeof(Driver_t));
      context->id = id;
      context->class = 0x#{@class};
      DriverTable[NumberOfDrivers].context = context;
      DriverTable[NumberOfDrivers].respond = Driver#{@name}Respond;
      DriverTable[NumberOfDrivers].poll = Driver#{@name}Poll;
      ++NumberOfDrivers;

      Driver#{@name}Init(context);
    \}
  }

  #Footer
  out "#endif __#{@name}_H_"

  #Write to file
  @file = File.open(driverHeaderFileName, "w+")
  @file << @fileBuffer
  @file.close()
  puts "##################################"
  puts ""
end

puts "Writing includes to drivers.h..."
File.open("#{Dir.pwd}/../src/drivers.h", "w+") do |file|
  file << "#ifndef DRIVER_H_\n"
  file << "#define DRIVER_H_\n"
  file << %{
    #include "debug.h"
    #include "i2cMaster.h"
    #include "my_uart.h"
    
    unsigned char DriverHeap[10];
    unsigned char *heapPointer = DriverHeap;
    unsigned char *driverMalloc(int size) {
      unsigned char *temp = heapPointer;

      heapPointer += size;

      return temp;
    }

    //Driver dispatch table
    //Id -> Driver Function?
    typedef struct DRIVER_T {
        unsigned char id;  //1 Byte
        unsigned char class; //1 Byte
        unsigned char data[10]; //Do whatever
    } Driver_t;

    typedef struct RRPACKET_T {
        unsigned char id; //1 Byte
        unsigned char class; //1 Byte
        unsigned char data[4]; //Payload
    } RRPacket_t;

    //Device ID Dispatch table (Convert a device ID to it's driver functions)
    typedef struct DRIVER_TABLE_ENT_T {
        unsigned char id;
        Driver_t *context;
        void (*respond)(Driver_t *, unsigned char *);
        void (*poll)(Driver_t *);
    } Driver_Table_Ent_t;

    #define MAX_DRIVERS 10
    Driver_Table_Ent_t DriverTable[MAX_DRIVERS];
    unsigned char NumberOfDrivers = 0;  //Current number of drivers
  }
  driverHeaderFileNames.each do |fn|
    file << "#include \"#{fn}\"\n"
  end

  file << "#endif DRIVER_H_\n"
  file.close()
end
