//! @file PIDcontroller.cc
//! @brief PID制御器クラス
//!
//! PID制御器 G(s) = Kp + Ki/s + Kd*s*gdis/(s+gdis) (双一次変換)
//!
//! @date 2019/07/29
//! @author Yuki YOKOKURA
//
// Copyright (C) 2011-2019 Yuki YOKOKURA
// This program is free software;
// you can redistribute it and/or modify it under the terms of the FreeBSD License.
// For details, see the License.txt file.

#include "PIDcontroller.h"

using namespace ARCS;

//! @brief コンストラクタ
//! @param[in]	Pgain	比例ゲイン
//! @param[in]	Igain	積分ゲイン
//! @param[in]	Dgain	微分ゲイン
//! @param[in]	SmplTime	[s] 制御周期
PIDcontroller::PIDcontroller(double Pgain, double Igain, double Dgain, double Bandwidth, double SmplTime)
	: Ts(SmplTime),		// [s]		制御周期の格納
	  Kp(Pgain),		// 			比例ゲインの格納
	  Ki(Igain),		// 			積分ゲインの格納
	  Kd(Dgain),		// 			微分ゲインの格納
	  gpd(Bandwidth),	// [rad/s]	擬似微分の帯域の格納
	  uZ1(0), uZ2(0), yZ1(0), yZ2(0)
{
}

//! @brief ムーブコンストラクタ
//! @param[in]	r	右辺値
PIDcontroller::PIDcontroller(PIDcontroller&& r)
	: Ts(r.Ts),		// [s]		制御周期の格納
	  Kp(r.Kp),		// 			比例ゲインの格納
	  Ki(r.Ki),		// 			積分ゲインの格納
	  Kd(r.Kd),		// 			微分ゲインの格納
	  gpd(r.gpd),	// [rad/s]	擬似微分の帯域の格納
	  uZ1(r.uZ1), uZ2(r.uZ2), yZ1(r.yZ1), yZ2(r.yZ2)
{
	
}

//! @brief デストラクタ
PIDcontroller::~PIDcontroller(){
}

//! @brief 出力信号の取得
//! @param[in]	u	PID制御器入力
//! @return	PID制御器出力
double PIDcontroller::GetSignal(double u){
	double y = ( 2.0*Ts*(Ki+Kp*gpd)*(u-uZ2) + Ki*gpd*Ts*Ts*(u+2.0*uZ1+uZ2) + 4.0*(Kd*gpd+Kp)*(u-2.0*uZ1+uZ2) - (4.0-2.0*gpd*Ts)*yZ2 + 8.0*yZ1 )/(4.0+2.0*gpd*Ts);
	
	uZ2 = uZ1;
	uZ1 = u;
	yZ2 = yZ1;
	yZ1 = y;
	
	return y;
}
