<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="DIGITAL_IO" URI="http://resources/4.0.6/app/DIGITAL_IO/5" description="The DIGITAL_IO APP is used to configure &#xA; a port pin as digital Input/Output." mode="NOTSHARABLE" version="4.0.6" minDaveVersion="4.0.0" instanceLabel="Q2ONP22" appLabel="">
  <properties provideInit="true"/>
  <virtualSignals name="software controlled output signal" URI="http://resources/4.0.6/app/DIGITAL_IO/5/vs_digital_io_sw_pin" hwSignal="swreg_out" hwResource="//@hwResources.0" required="false"/>
  <virtualSignals name="pin" URI="http://resources/4.0.6/app/DIGITAL_IO/5/vs_digital_io_pad_pin" hwSignal="pad" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="pin_signal" URI="http://resources/4.0.6/app/DIGITAL_IO/5/__pin_vs_digital_io_pad_pin" hwSignal="pin" hwResource="//@hwResources.1"/>
  <hwResources name="pin" URI="http://resources/4.0.6/app/DIGITAL_IO/5/hwres_port_pin" resourceGroupUri="port/p/sv0/pad/*" solverVariable="true" mResGrpUri="port/p/sv0/pad/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/Port2/Port2_2.dd#//@provided.3"/>
    <solverVarMap index="2">
      <value variableName="sv0" solverValue="2"/>
    </solverVarMap>
    <solverVarMap index="2">
      <value variableName="sv0" solverValue="2"/>
    </solverVarMap>
  </hwResources>
  <hwResources name="pin" URI="http://resources/4.0.6/app/DIGITAL_IO/5/__pin_hwres_port_pin" resourceGroupUri="devicepackage/0/24" constraintType="GLOBAL_RESOURCE" mResGrpUri="devicepackage/0/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/DEVICEPACKAGE/DEVICEPACKAGE_0.dd#//@provided.8"/>
  </hwResources>
  <connections URI="http://resources/4.0.6/app/DIGITAL_IO/5/http://resources/4.0.6/app/DIGITAL_IO/5/vs_digital_io_pad_pin/http://resources/4.0.6/app/DIGITAL_IO/5/__pin_vs_digital_io_pad_pin" systemDefined="true" sourceSignal="pin" targetSignal="pin_signal" srcVirtualSignal="//@virtualSignals.1" targetVirtualSignal="//@virtualSignals.2"/>
  <connections URI="http://resources/4.0.6/app/DIGITAL_IO/5/http://resources/4.0.6/app/DIGITAL_IO/5/__pin_vs_digital_io_pad_pin/http://resources/4.0.6/app/DIGITAL_IO/5/vs_digital_io_pad_pin" systemDefined="true" sourceSignal="pin_signal" targetSignal="pin" srcVirtualSignal="//@virtualSignals.2" targetVirtualSignal="//@virtualSignals.1"/>
  <connections URI="http://resources/4.0.6/app/DIGITAL_IO/5/http://resources/4.0.6/app/DIGITAL_IO/5/vs_digital_io_pad_pin/http://resources/4.1.4/app/COUNTER/1/vs_counter_ccu4_input_signal" sourceSignal="pin" targetSignal="counter_input" srcVirtualSignal="//@virtualSignals.1">
    <downwardMapList xsi:type="ResourceModel:VirtualSignal" href="../../COUNTER/v4_1_4/COUNTER_1.app#//@virtualSignals.14"/>
    <targetVirtualSignal href="../../COUNTER/v4_1_4/COUNTER_1.app#//@virtualSignals.14"/>
  </connections>
</ResourceModel:App>
