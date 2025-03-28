#ifndef __CEXKIT_BINANCE_DATAHUB_HPP__
#define __CEXKIT_BINANCE_DATAHUB_HPP__

#include <iostream>
#include <map>
#include <set>
#include <string>

#include "cexkit/binance/orderbook.hpp"
#include "cexkit/utils.hpp"
#include "netkit/rest.hpp"
#include "netkit/websocket.hpp"

namespace cexkit {

namespace binance {

class DataHub {
 private:
  int maxNumStreams_ = 10;
  uint64_t eventBufferSize_ = 100;

  // It's not DatHub's responsibility to delete rest_ and ws_.
  netkit::Rest *rest_;
  netkit::Websocket *ws_;

  std::set<std::string> streams_;  // Subscribed streams.
  std::map<std::string, OrderBook *> orderbooks_;

 private:
  /**
   * * Callback updating local orderbook through event received from Diff.Depth
   * * Stream. This callback assumes the OrderBook object has been instantiated.
   * * And the caller needs to make sure the payload of passed frame contains
   * * the "stream" field.
   */
  void updateOBByEvent(netkit::Frame frame);

 public:
  DataHub(netkit::Rest *rest, netkit::Websocket *ws);
  ~DataHub();

  void join();

  // Subscribe a stream (streams) through websocket.
  void subscribe(const std::string &stream);
  void subscribe(const std::vector<std::string> &streams);

  // Unsubscribe a stream (streams) on websocket.
  void unsubscribe(const std::string &stream);
  void unsubscribe(const std::vector<std::string> &streams);

  void listSubscriptopns();

  void subscribeOrderBook(const std::string &symbol);
  void unsubscribeOrderBook(const std::string &symbol);
  OrderBook *orderbook(const std::string &symbol);

  // Set the maximum number of streams one websocket can listen.
  void setMaxNumStreams(int val) { maxNumStreams_ = val; }

  void registerCallback(const std::string &stream,
                        const std::function<void(netkit::Frame)> &);
};  // DataHub

}  // namespace binance

}  // namespace cexkit

#endif