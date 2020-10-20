#include <sick_safetyscanners_base/Exceptions.h>
#include <sick_safetyscanners_base/SickSafetyscanners.h>
#include <sick_safetyscanners_base/Types.h>
#include <sick_safetyscanners_base/datastructure/CommSettings.h>

int main(int argc, char *argv[]) {
  // Sensor IP and Port
  sick::types::ip_address_t sensor_ip = sick::types::ip_address_t::from_string("192.168.10.16");
  sick::types::port_t tcp_port = 2122;

  // Prepare the CommSettings for Sensor streaming data
  sick::datastructure::CommSettings comm_settings;
  comm_settings.host_ip =
      boost::asio::ip::address_v4::from_string("192.168.10.5");
  comm_settings.host_udp_port = 6060;

  // Define a sensor data callback
  sick::types::ScanDataCb cb = [](const sick::datastructure::Data &data) {
    std::cout << "Number of beams: "
              << data.getMeasurementDataPtr()->getNumberOfBeams() << std::endl;
  };

  // Create a sensor instance
      sick::AsyncSickSafetyScanner safety_scanner(sensor_ip, tcp_port, comm_settings, cb);

  // Start async receiving and processing of sensor data
  safety_scanner.run();

  // ... Do other stuff
  printf("press anykey to quit.\r\n");
  getchar();

  // Stop async processing
  safety_scanner.stop();
  return 0;
}
