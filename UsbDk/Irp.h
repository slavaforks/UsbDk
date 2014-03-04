#pragma once

#include "ntddk.h"
#include "Alloc.h"
#include "UsbDkUtil.h"

class CIrp : public CAllocatable<NonPagedPool, 'RIHR'>
{
public:
    CIrp() {};
    ~CIrp();

    NTSTATUS Create(PDEVICE_OBJECT TargetDevice);
    void Destroy();
    NTSTATUS SendSynchronously();

    template<typename ConfiguratorT>
    void Configure(ConfiguratorT Configurator)
    { Configurator(IoGetNextIrpStackLocation(m_Irp)); }

    template<typename ReaderT>
    void ReadResult(ReaderT Reader)
    { Reader(m_Irp->IoStatus.Information); }

    CIrp(const CIrp&) = delete;
    CIrp& operator= (const CIrp&) = delete;

private:
    static NTSTATUS SynchronousCompletion(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context);
    void DestroyIrp();
    void ReleaseTarget();

    PIRP m_Irp = nullptr;
    PDEVICE_OBJECT m_TargetDevice = nullptr;
};
//-------------------------------------------------------------------------------------------------

class CIoControlIrp : public CAllocatable<NonPagedPool, 'CIHR'>
{
public:
    CIoControlIrp() {}
    ~CIoControlIrp();

    NTSTATUS Create(PDEVICE_OBJECT TargetDevice,
                    ULONG IoControlCode,
                    bool IsInternal = true,
                    PVOID InputBuffer = nullptr,
                    ULONG InputBufferLength = 0,
                    PVOID OutputBuffer = nullptr,
                    ULONG OutputBufferLength = 0);
    void Destroy();
    NTSTATUS SendSynchronously();

    CIoControlIrp(const CIoControlIrp&) = delete;
    CIoControlIrp& operator= (const CIoControlIrp&) = delete;

private:
    PIRP m_Irp = nullptr;
    PDEVICE_OBJECT m_TargetDevice = nullptr;
    CWdmEvent m_Event;
    IO_STATUS_BLOCK m_IoControlStatus;
};
//-------------------------------------------------------------------------------------------------
