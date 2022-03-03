#ifndef PIDCTRLLER
#define PIDCTRLLER

namespace ARCS {	// ARCS名前空間
	//! @brief PID制御器クラス
	class PIDcontroller {
		public:
			PIDcontroller(double Pgain, double Igain, double Dgain, double Bandwidth, double SmplTime);	//!< コンストラクタ
			PIDcontroller(PIDcontroller&& r);	//!< ムーブコンストラクタ
			~PIDcontroller();					//!< デストラクタ
			double GetSignal(double u);			//!< 出力信号の取得 u；入力信号
			
		private:
			PIDcontroller(const PIDcontroller&) = delete;					//!< コピーコンストラクタ使用禁止
			const PIDcontroller& operator=(const PIDcontroller&) = delete;	//!< 代入演算子使用禁止
			double Ts;	//!< [s]		制御周期
			double Kp;	//!<			比例ゲイン
			double Ki;	//!<			積分ゲイン
			double Kd;	//!<			微分ゲイン
			double gpd;	//!< [rad/s]	擬似微分器の帯域
			double uZ1;	//!<			状態変数1 変数名Z1の意味はz変換のz^(-1)を示す
			double uZ2;	//!<			状態変数2
			double yZ1;	//!<			状態変数3
			double yZ2;	//!<			状態変数4
	};
}

#endif
