<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="DMA_CH" URI="http://resources/4.1.4/app/DMA_CH/1" description="The DMA_CH APP is used to perform single and multi-block data transfer using the GPDMA module on XMC4000" version="4.1.4" minDaveVersion="4.0.0" instanceLabel="DMA_CH_IFI" appLabel="" containingProxySignal="true">
  <properties provideInit="true" sharable="true"/>
  <virtualSignals name="Channel event" URI="http://resources/4.1.4/app/DMA_CH/1/event" hwSignal="dma_int" hwResource="//@hwResources.0"/>
  <virtualSignals name="src_req" URI="http://resources/4.1.4/app/DMA_CH/1/channel_src_req" hwSignal="src_req" hwResource="//@hwResources.0"/>
  <virtualSignals name="dest_req" URI="http://resources/4.1.4/app/DMA_CH/1/channel_dest_req" hwSignal="dest_req" hwResource="//@hwResources.0"/>
  <virtualSignals name="source_trigger" URI="http://resources/4.1.4/app/DMA_CH/1/src_peri_trig" hwSignal="in" hwResource="//@hwResources.1" visible="true">
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT/v4_1_6/ADC_MEASUREMENT_0.app#//@connections.132"/>
  </virtualSignals>
  <virtualSignals name="dlr_out" URI="http://resources/4.1.4/app/DMA_CH/1/src_trigger_out" hwSignal="out" hwResource="//@hwResources.1"/>
  <virtualSignals name="destination_trigger" URI="http://resources/4.1.4/app/DMA_CH/1/dst_peri_trig" hwSignal="in" hwResource="//@hwResources.2" required="false" visible="true"/>
  <virtualSignals name="dlr_out" URI="http://resources/4.1.4/app/DMA_CH/1/dest_trigger_out" hwSignal="out" hwResource="//@hwResources.2" required="false"/>
  <requiredApps URI="http://resources/4.1.4/app/DMA_CH/1/ra_nvic_dma" requiredAppName="GLOBAL_DMA" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../GLOBAL_DMA/v4_0_4/GLOBAL_DMA_0.app#/"/>
  </requiredApps>
  <hwResources name="DMA Channel" URI="http://resources/4.1.4/app/DMA_CH/1/rg_channel" resourceGroupUri="peripheral/sv0/0/sv1/*" solverVariable="true" mResGrpUri="peripheral/sv0/0/sv1/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/GPDMA0/GPDMA0_0.dd#//@provided.0"/>
    <solverVarMap index="1">
      <value variableName="sv0" solverValue="dma8"/>
    </solverVarMap>
    <solverVarMap index="3">
      <value variableName="sv1" solverValue="ch_adv"/>
    </solverVarMap>
    <solverVarMap index="1">
      <value variableName="sv0" solverValue="dma8"/>
    </solverVarMap>
    <solverVarMap index="3">
      <value variableName="sv1" solverValue="ch_adv"/>
    </solverVarMap>
  </hwResources>
  <hwResources name="DMA DLR" URI="http://resources/4.1.4/app/DMA_CH/1/rg_src_dlr" resourceGroupUri="peripheral/dlr/0/ch/*" mResGrpUri="peripheral/dlr/0/ch/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/DLR/DLR_0.dd#//@provided.1"/>
  </hwResources>
  <hwResources name="DMA DLR" URI="http://resources/4.1.4/app/DMA_CH/1/rg_dest_dlr" resourceGroupUri="" required="false" mResGrpUri="peripheral/dlr/0/ch/*"/>
  <connections URI="http://resources/4.1.4/app/DMA_CH/1/http://resources/4.1.4/app/DMA_CH/1/event/http://resources/4.1.4/app/DMA_CH/1/nvic_dma_irq" systemDefined="true" sourceSignal="Channel event" targetSignal="Interrupt signal" srcVirtualSignal="//@virtualSignals.0" proxyTargetVirtualSignalUri="http://resources/4.0.4/app/GLOBAL_DMA/0/interrupt_dma" containingProxySignal="true">
    <downwardMapList xsi:type="ResourceModel:VirtualSignal" href="../../GLOBAL_DMA/v4_0_4/GLOBAL_DMA_0.app#//@virtualSignals.0"/>
    <targetVirtualSignal href="../../GLOBAL_DMA/v4_0_4/GLOBAL_DMA_0.app#//@virtualSignals.0"/>
  </connections>
  <connections URI="http://resources/4.1.4/app/DMA_CH/1/http://resources/4.1.4/app/DMA_CH/1/src_trigger_out/http://resources/4.1.4/app/DMA_CH/1/channel_src_req" systemDefined="true" sourceSignal="dlr_out" targetSignal="src_req" srcVirtualSignal="//@virtualSignals.4" targetVirtualSignal="//@virtualSignals.1"/>
  <connections URI="http://resources/4.1.4/app/DMA_CH/1/http://resources/4.1.4/app/DMA_CH/1/dest_trigger_out/http://resources/4.1.4/app/DMA_CH/1/channel_dest_req" systemDefined="true" sourceSignal="dlr_out" targetSignal="dest_req" required="false" srcVirtualSignal="//@virtualSignals.6" targetVirtualSignal="//@virtualSignals.2"/>
</ResourceModel:App>
