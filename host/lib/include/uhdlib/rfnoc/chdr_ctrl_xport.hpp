//
// Copyright 2019 Ettus Research, a National Instruments Brand
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#ifndef INCLUDED_RFNOC_CHDR_CTRL_XPORT_HPP
#define INCLUDED_RFNOC_CHDR_CTRL_XPORT_HPP

#include <uhdlib/rfnoc/chdr_types.hpp>
#include <uhdlib/transport/io_service.hpp>
#include <mutex>

namespace uhd { namespace rfnoc {

/*!
 * Represents a transport for CHDR control streams and management streams
 */
class chdr_ctrl_xport
{
public:
    using io_service   = uhd::transport::io_service;
    using frame_buff   = uhd::transport::frame_buff;
    using send_link_if = uhd::transport::send_link_if;
    using recv_link_if = uhd::transport::recv_link_if;
    using send_io_if   = uhd::transport::send_io_if;
    using recv_io_if   = uhd::transport::recv_io_if;

    using sptr = std::shared_ptr<chdr_ctrl_xport>;

    /*!
     * Make a chdr_ctrl_xport
     *
     * \param io_srv The I/O service that send_link and recv_link are attached to
     * \param send_link Link to use for sending packets
     * \param recv_link Link to use for receiving packets
     * \param my_epid The local EPID for this transport
     * \param num_send_frames Number of frames to reserve for TX
     * \param num_recv_frames Number of frames to reserve for RX
     */
    static sptr make(io_service::sptr io_srv,
        send_link_if::sptr send_link,
        recv_link_if::sptr recv_link,
        sep_id_t my_epid,
        size_t num_send_frames,
        size_t num_recv_frames)
    {
        return std::make_shared<chdr_ctrl_xport>(
            io_srv, send_link, recv_link, my_epid, num_send_frames, num_recv_frames);
    }

    /*!
     * Make a chdr_ctrl_xport
     *
     * \param io_srv The I/O service that send_link and recv_link are attached to
     * \param send_link Link to use for sending packets
     * \param recv_link Link to use for receiving packets
     * \param my_epid The local EPID for this transport
     * \param num_send_frames Number of frames to reserve for TX
     * \param num_recv_frames Number of frames to reserve for RX
     */
    chdr_ctrl_xport(io_service::sptr io_srv,
        send_link_if::sptr send_link,
        recv_link_if::sptr recv_link,
        sep_id_t my_epid,
        size_t num_send_frames,
        size_t num_recv_frames);

    ~chdr_ctrl_xport() = default;

    /*!
      * Get an empty frame buffer in which to write packet contents.
      *
      * \param timeout_ms a positive timeout value specifies the maximum number
                          of ms to wait, a negative value specifies to block
                          until successful, and a value of 0 specifies no wait.
      * \return a frame buffer, or null uptr if timeout occurs
      */
    frame_buff::uptr get_send_buff(int32_t timeout_ms);

    /*!
     * Release a frame buffer, allowing the driver to reuse it.
     *
     * \param buffer frame buffer to release for reuse by the link
     */
    void release_send_buff(frame_buff::uptr buff);

    /*!
     * Attempt to get a frame buffer with data from the recv link.
     *
     * Receives packets from the control stream.
     *
     * \param timeout_ms a positive timeout value specifies the maximum number
                         of ms to wait, a negative value specifies to block
                         until successful, and a value of 0 specifies no wait.
     * \return a frame buffer, or null uptr if timeout occurs
     */
    frame_buff::uptr get_recv_buff(int32_t timeout_ms);

    /*!
     * FIXME: Remove or alter when get threaded_io_service
     *
     * Receives packets from the management stream.
     */
    frame_buff::uptr get_mgmt_buff(int32_t timeout_ms);

    /*!
     * Release a frame buffer, allowing the recv link driver to reuse it.
     *
     * \param buffer frame buffer to release for reuse by the link
     */
    void release_recv_buff(frame_buff::uptr buff);

    /*!
     * Get this xport's EPID
     *
     * \return the source EPID for this transport
     */
    sep_id_t get_epid() const;

private:
    chdr_ctrl_xport(const chdr_ctrl_xport&) = delete;

    sep_id_t _my_epid;
    send_io_if::sptr _send_if;
    recv_io_if::sptr _ctrl_recv_if;
    recv_io_if::sptr _mgmt_recv_if;

    // FIXME: Remove this when have threaded_io_service
    std::mutex _mutex;
};

}} // namespace uhd::rfnoc

#endif /* INCLUDED_UHDLIB_RFNOC_CHDR_CTRL_XPORT_HPP */