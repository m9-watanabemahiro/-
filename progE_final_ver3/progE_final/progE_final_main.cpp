#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <random>
#include <string>
using namespace std;

class Ability {
protected:
	//ゲーム要素の定義

	//ステータス(HP,クリティカルゲージ,恐怖値)
	int hp;			//0~3
	int c_gauge;	//0~3
	int fear_value;	//0~100

public:
	void sethp(int& newhp) { hp = newhp; }
	int gethp() { return hp; }
	void setc_gauge(const int& newc_gauge) { c_gauge = newc_gauge; }
	int getc_gauge() { return c_gauge; }
	void setfear_value(const int& newfear_value) { fear_value = newfear_value; }
	int getfear_value() { return fear_value; }

};

//CPUコマンド,攻撃側,クリティカルなし
int CPU_4A_command(int a, int b, int c) {
	random_device engine;
	uniform_int_distribution<signed> dist1(0, 99);
	int x = dist1(engine);
	if (x < a) {
		return 0;
	}
	else if (a <= x && x < b) {
		return 1;
	}
	else if (b <= x && x < c) {
		return 2;
	}
	else {
		return 3;
	}
}

//CPUコマンド,防御側,クリティカルなし
int CPU_4D_command(int a, int b, int c) {
	random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 99);
	int x = dist1(engine);
	if (x < a) {
		return 0;
	}	
	else if (a <= x && x < b) {
		return 1;
		}
	else if (b <= x && x < c) {
		return 2;
		}	
	else {
		return 3;		
	}
}

//CPUコマンド,攻撃側,クリティカルあり
int CPU_5A_command(int a, int b, int c, int d) {
	random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 99);
	int x = dist1(engine);
	if (x < a) {
		return 0;
	}
	else if (a <= x && x < b) {
		return 1;
	}
	else if (b <= x && x < c) {
		return 2;
	}
	else if (c <= x && x < d) {
		return 3;
	}
	else {
		return 4;
	}
}

//CPUコマンド,防御側,クリティカルあり
int CPU_5D_command(int a, int b, int c, int d) {
	random_device engine;
	uniform_int_distribution<signed> dist1(0, 99);
	int x = dist1(engine);
	if (x < a) {
		return 0;
	}
	else if (a <= x && x < b) {
		return 1;
	}
	else if (b <= x && x < c) {
		return 2;
	}
	else if (c <= x && x < d) {
		return 3;
	}
	else {
		return 4;
	}
}

int Counter(){
	random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 9);
	int x = dist1(engine);
	if (x < 5) {
		cout << "カウンター成功" << endl;
		return 1;
	}
	else {
		cout << "カウンター失敗" << endl;
		return 0;
	}
}

int Threaten_dist() {
	random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 9);
	int x = dist1(engine);
	if (x < 5) {
		cout << "脅す成功" << endl;
		return 1;
	}
	else {
		cout << "脅す失敗" << endl;
		return 0;
	}
}

int Gaze_dist() {
	random_device engine;
	uniform_int_distribution<unsigned> dist1(0, 9);
	int x = dist1(engine);
	if (x < 7) {
		cout << "睨む成功" << endl;
		return 1;
	}
	else {
		cout << "睨む失敗" << endl;
		return 0;
	}
}


int main(){

	//初期ステータス設定
	//vector管理,CPU[0],Player[1]
	vector<int> v_hp(2);
	int h = 3;		//初期HP
	v_hp[0] = h;
	v_hp[1] = h;
	vector<int> v_c_gauge(2);
	int cric = 0;		//初期クリティカルゲージ
	v_c_gauge[0] = cric;
	v_c_gauge[1] = cric;
	vector<int> v_fear_value(2);
	int  frv = 50;		//初期恐怖値
	v_fear_value[0] = frv;
	v_fear_value[1] = frv;

	//vector格納
	vector<Ability> v_all;
	//要素の追加（空）
	size_t t = 0;
	for(size_t s = 0; s < 2; s++) v_all.emplace_back();
	//要素に登録
	for (size_t i = 0; i < v_all.size(); i++) {
		v_all[i].sethp(v_hp[i]);
		v_all[i].setc_gauge(v_c_gauge[i]);
		v_all[i].setfear_value(v_fear_value[i]);
	}

	//変数設定
	int CPU_hp = 0;
	int Player_hp = 0;
	int CPU_c_gauge = 0;
	int Player_c_gauge = 0;
	int CPU_fear_value = 0;
	int Player_fear_value = 0;
	int tn = 0;					//ターン数用
	int suc = 0;				//会話用
	int suc_c = 0;				//カウンター用
	int command = 0;

	cout << "GAMEPLAY" << endl;
	cout << endl;

	//イテレータによる初期状態表示
	auto it0 = v_hp.begin();
	cout << ">PlayerHP : " << *it0 << endl;
	auto it1 = v_c_gauge.begin();
	cout << ">Playerクリティカルゲージ : " << *it1 << endl;
	auto it2 = v_fear_value.begin();
	cout << ">Player恐怖値 : " << *it2 << endl;
	cout << endl;
	cout << ">CPUHP : " << *(it0 + 1) << endl;
	cout << ">CPUクリティカルゲージ : " << *(it1 + 1) << endl;
	cout << ">CPU恐怖値 : " << *(it2 + 1) << endl;
	cout << endl;

	while (1) {

		//if CPUのhp0 = 勝ちGAMECLEAR
		if (v_all[0].gethp() <= 0) {
			cout << "GAMECLEAR" << endl;
			return 0;
		}
		//else if Playerのhp0 = 負けGAMEOVER
		else if (v_all[1].gethp() <= 0) {
			cout << "GAMEOVER" << endl;
			return 0;
		}
		//else hp>0 = コマンド入力へ
		else {
			//ターン数処理
			tn = tn + 1;
			cout << "ターン : " << tn << endl;

			//if　1ターン目or奇数ターン = 攻撃側
			if (tn % 2 == 1) {
				cout << "＋攻撃側＋" << endl;
				
				int Player = command;
				int CPU = 0;

				//CPU防御_行動決定_コマンド選択確率
				if (Player_fear_value >= 100) {
					CPU = 0;
				}
				else {
					//Player(3or2,0or1)_4D
					if ((Player_hp == 3 || Player_hp == 2) && (Player_c_gauge == 0 || Player_c_gauge == 1)) {
						//if CPU(3,0or1), 0[40%], 6[10%], 7[10%], 8[40%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(40, 50, 60);
						}
						//if CPU(2,0or1), 0[30%], 6[20%], 7[20%], 8[30%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(30, 50, 70);
						}
						//if CPU(1,0or1), 0[1%], 6[33%], 7[33%], 8[33%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(1, 34, 67);
						}
						//if CPU(2or3,2), 0[41%], 6[27%], 7[27%], 8[5%]
						else if ((CPU_hp == 2|| CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4D_command(41, 68, 95);
						}
						//if CPU(1,2), 0[15%], 6[40%], 7[40%], 8[5%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4D_command(15, 55, 95);
						}
						//if CPU(2or3,3), 0[0%], 6[49%], 7[49%], 8[2%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_4D_command(0, 49, 98);
						}
						//if CPU(1,3), 0[0%], 6[50%], 7[50%], 8[0%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_4D_command(0, 50, 100);
						}
					}

					//Player(3or2,2)_4D
					else if ((Player_hp == 3 || Player_hp == 2) && Player_c_gauge == 2) {
						//if CPU(3,0or1), 0[80%], 6[8%], 7[8%], 8[4%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(80, 88, 96);
						}
						//if CPU(2,0or1), 0[60%], 6[10%], 7[10%], 8[20%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(60, 70, 80);
						}
						//if CPU(1,0or1), 0[1%], 6[33%], 7[33%], 8[33%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(1, 34, 67);
						}
						//if CPU(2or3,2), 0[80%], 6[5%], 7[5%], 8[10%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4D_command(80, 85, 90);
						}
						//if CPU(1,2), 0[15%], 6[40%], 7[40%], 8[5%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4D_command(15, 55, 95);
						}
						//if CPU(2or3,3), 0[0%], 6[49%], 7[49%], 8[2%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_4D_command(0, 49, 98);
						}
						//if CPU(1,3), 0[0%], 6[50%], 7[50%], 8[0%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_4D_command(0, 50, 100);
						}
					}

					//Player(3or2,3)_5D
					else if ((Player_hp == 3 || Player_hp == 2) && Player_c_gauge >= 3) {
						//if CPU(3,0or1), 0[10%], 6[0%], 7[0%], 8[10%], 10[80%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_5D_command(10, 10, 10, 20);
						}
						//if CPU(2,0or1), 0[10%], 6[0%], 7[0%], 8[10%], 10[80%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_5D_command(10, 10, 10, 20);
						}
						//if CPU(1,0or1), 0[0%], 6[0%], 7[0%], 8[50%], 10[50%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_5D_command(0, 0, 0, 50);
						}
						//if CPU(2or3,2), 0[15%], 6[0%], 7[0%], 8[5%], 10[80%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_5D_command(15, 15, 15, 20);
						}
						//if CPU(1,2), 0[0%], 6[0%], 7[0%], 8[50%], 10[50%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_5D_command(0, 0, 0, 50);
						}
						//if CPU(2or3,3), 0[0%], 6[0%], 7[0%], 8[5%], 10[95%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5D_command(0, 0, 0, 5);
						}
						//if CPU(1,3), 0[0%], 6[0%], 7[0%], 8[50%], 10[50%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5D_command(0, 0, 0, 50);
						}
					}

					//Player(1,0or1or2)_4D
					else if (Player_hp == 1 && (Player_c_gauge == 0 || Player_c_gauge == 1 || Player_c_gauge == 2)) {
						//if CPU(3,0or1), 50[%], 6[5%], 7[5%], 8[40%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(50, 55, 60);
						}
						//if CPU(2,0or1), 0[40%], 6[5%], 7[5%], 8[50%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(40, 45, 50);
						}
						//if CPU(1,0or1), 0[1%], 6[15%], 7[15%], 8[69%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4D_command(1, 16, 31);
						}
						//if CPU(2or3,2), 0[50%], 6[0%], 7[0%], 8[50%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4D_command(50, 50, 50);
						}
						//if CPU(1,2), 0[50%], 6[0%], 7[0%], 8[50%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4D_command(50, 50, 50);
						}
						//if CPU(2or3,3), 0[0%], 6[1%], 7[1%], 8[98%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_4D_command(0, 1, 2);
						}
						//if CPU(1,3), 0[0%], 6[1%], 7[1%], 8[98%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_4D_command(0, 1, 2);
						}
					}

					//Player(1,3)_5D
					else if (Player_hp == 1 && Player_c_gauge >= 3) {
						//if CPU(3,0or1), 0[1%], 6[0%], 7[0%], 8[9%], 10[90%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_5D_command(1, 1, 1, 10);
						}
						//if CPU(2,0or1), 0[1%], 6[0%], 7[0%], 8[9%], 10[90%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_5D_command(1, 1, 1, 10);
						}
						//if CPU(1,0or1), 0[0%], 6[0%], 7[0%], 8[50%], 10[50%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_5D_command(0, 0, 0, 50);
						}
						//if CPU(2or3,2), 0[1%], 6[0%], 7[0%], 8[9%], 10[90%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_5D_command(1, 1, 1, 10);
						}
						//if CPU(1,2), 0[0%], 6[0%], 7[0%], 8[50%], 10[50%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_5D_command(0, 0, 0, 50);
						}
						//if CPU(2or3,3), 0[0%], 6[0%], 7[0%], 8[5%], 10[95%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5D_command(0, 0, 0, 5);
						}
						//if CPU(1,3), 0[0%], 6[0%], 7[0%], 8[50%], 10[50%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5D_command(0, 0, 0, 50);
						}
					}
					
					//予備
					else {
						CPU = CPU_4D_command(40, 60, 80);
					}
				}

				//Player恐怖値100以上で停止
				if (Player_fear_value>=100){
					  cout << "Playerは動けない" << endl;
					  Player = 9;
					  Player_fear_value = 0;
					  v_all[1].setfear_value(Player_fear_value);
				  }
				//CPU恐怖値100以上で停止 & Player行動決定
				else if (CPU_fear_value >= 100) {
					  cout << "CPUは動けない" << endl;
					  CPU = 9;	//何もできないコマンド
					  CPU_fear_value = 0;
					  v_all[0].setfear_value(CPU_fear_value);

					  //クリティカルゲージ判定
					  if (Player_c_gauge >= 3) {
						  //Playerにクリティカル攻撃許可
						  cout << "クリティカル攻撃出現!" << endl;
						  cout << "コマンドを選択せよ" << endl;
						  cout << ">>ためる[0], 上段攻撃[1], 下段攻撃[2], 会話[3], クリティカル攻撃[4]" << endl;
						  cin >> command;
						  Player = command;
						  if (Player == 0)Player = 0;
						  else if (Player == 1)Player = 1;
						  else if (Player == 2)Player = 2;
						  else if (Player == 3)Player = 3;
						  else if (Player == 4)Player = 4;
						  else Player = 9;
					  }
					  else {
						  //通常コマンド表示
						  cout << "コマンドを選択せよ" << endl;
						  cout << ">>ためる[0], 上段攻撃[1], 下段攻撃[2], 会話[3]" << endl;
						  cin >> command;
						  Player = command;
						  if (Player == 0)Player = 0;
						  else if (Player == 1)Player = 1;
						  else if (Player == 2)Player = 2;
						  else if (Player == 3)Player = 3;
						  else Player = 9;
					  }
				  }
				//else
				else {
					  if (Player_c_gauge >= 3) {
						  //Playerにクリティカル攻撃許可
						  cout << "クリティカル攻撃出現!" << endl;
						  cout << "コマンドを選択せよ" << endl;
						  cout << ">>ためる[0], 上段攻撃[1], 下段攻撃[2], 会話[3], クリティカル攻撃[4]" << endl;
						  cin >> command;
						  Player = command;
						  if (Player == 0)Player = 0;
						  else if (Player == 1)Player = 1;
						  else if (Player == 2)Player = 2;
						  else if (Player == 3)Player = 3;
						  else if (Player == 4)Player = 4;
						  else Player = 9;
					  }
					  else {
						  //通常コマンド表示
						  cout << "コマンドを選択せよ" << endl;
						  cout << ">>ためる[0], 上段攻撃[1], 下段攻撃[2], 会話[3]" << endl;
						  cin >> command;
						  Player = command;
						  if (Player == 0)Player = 0;
						  else if (Player == 1)Player = 1;
						  else if (Player == 2)Player = 2;
						  else if (Player == 3)Player = 3;
						  else Player = 9;
					  }
				  }				  

				//リザルト

				//CPU(ためる) vs Player(ためる)
				if (CPU == 0 && Player == 0) {
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(ためる) vs Player(ためる)" << endl;
				}

				//CPU(ためる) vs Player(上段攻撃)
				else if (CPU == 0 && Player == 1) {
					//HP処理
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(ためる) vs Player(上段攻撃)" << endl;
				}

				//CPU(ためる) vs Player(下段攻撃)
				else if (CPU == 0 && Player == 2) {
					//HP処理
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(ためる) vs Player(下段攻撃)" << endl;
				}

				//CPU(ためる) vs Player(会話)
				else if (CPU == 0 && Player == 3) {
					//ゲーム画面処理
					cout << "CPU(ためる) vs Player(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					cin >> chat_com;	//脅す[0]or睨む[1]
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 50;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 30;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					//CPU_fear_value = v_all[0].getfear_value();
					//Player_fear_value = v_all[1].getfear_value();
				}

				//CPU(しゃがむ) vs Player(ためる)
				else if (CPU == 1 && Player == 0) {
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(しゃがむ) vs Player(ためる)" << endl;
				}

				//CPU(しゃがむ) vs Player(上段攻撃)
				else if (CPU == 1 && Player == 1) {
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(しゃがむ) vs Player(上段攻撃)" << endl;
				}

				//CPU(しゃがむ) vs Player(下段攻撃)
				else if (CPU == 1 && Player == 2) {
					//HP処理
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(しゃがむ) vs Player(下段攻撃)" << endl;
				}

				//CPU(しゃがむ) vs Player(会話)
				else if (CPU == 1 && Player == 3) {
					//ゲーム画面処理
					cout << "CPU(しゃがむ) vs Player(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					cin >> chat_com;	//脅す[0]or睨む[1]
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 50;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 30;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//CPU(ジャンプ) vs Player(ためる)
				else if (CPU == 2 && Player == 0) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge() + 1;
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(ジャンプ) vs Player(ためる)" << endl;
				}

				//CPU(ジャンプ) vs Player(上段攻撃)
				else if (CPU == 2 && Player == 1) {
				//HP処理
				CPU_hp = v_all[0].gethp() - 1;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(ジャンプ) vs Player(上段攻撃)" << endl;
				}

				//CPU(ジャンプ) vs Player(下段攻撃)
				else if (CPU == 2 && Player == 2) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(ジャンプ) vs Player(下段攻撃)" << endl;
				}

				//CPU(ジャンプ) vs Player(会話)
				else if (CPU == 2 && Player == 3) {
					//ゲーム画面処理
					cout << "CPU(ジャンプ) vs Player(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					cin >> chat_com;	//脅す[0]or睨む[1]
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 50;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 30;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//CPU(カウンター) vs Player(ためる)
				else if (CPU == 3 && Player == 0) {
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(カウンター) vs Player(ためる)" << endl;
				}

				//CPU(カウンター) vs Player(上段攻撃)
				else if (CPU == 3 && Player == 1) {
				cout << "CPU(カウンター) vs Player(上段攻撃)" << endl;
				suc_c = Counter();
				if (suc_c == 1) {
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp() - 1;
				}
				else if(suc_c == 0){
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
				}
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				}

				//CPU(カウンター) vs Player(下段攻撃)
				else if (CPU == 3 && Player == 2) {
				cout << "CPU(カウンター) vs Player(下段攻撃)" << endl;
				suc_c = Counter();
				if (suc_c == 1) {
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp() - 1;
				}
				else if (suc_c == 0) {
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
				}
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge() + 1;
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				}

				//CPU(カウンター) vs Player(会話)
				else if (CPU == 3 && Player == 3) {
					//ゲーム画面処理
					cout << "CPU(カウンター) vs Player(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					cin >> chat_com;	//脅す[0]or睨む[1]
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 50;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 30;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//クリティカル
				//CPU(クリティカル防御) vs Player(ためる)
				else if (CPU == 4 && Player == 0) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge() + 1;
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(クリティカル防御) vs Player(ためる)" << endl;
				}

				//CPU(クリティカル防御) vs Player(上段攻撃)
				else if (CPU == 4 && Player == 1) {
				//HP処理
				CPU_hp = v_all[0].gethp() - 1;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(クリティカル防御) vs Player(上段攻撃)" << endl;
				}

				//CPU(クリティカル防御) vs Player(下段攻撃)
				else if (CPU == 4 && Player == 2) {
				//HP処理
				CPU_hp = v_all[0].gethp() - 1;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(クリティカル防御) vs Player(下段攻撃)" << endl;
				}

				//CPU(クリティカル防御) vs Player(会話)
				else if (CPU == 4 && Player == 3) {
					//ゲーム画面処理
					cout << "CPU(クリティカル防御) vs Player(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					cin >> chat_com;	//脅す[0]or睨む[1]
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 50;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 30;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//CPU(クリティカル防御) vs Player(クリティカル攻撃)
				else if (CPU == 4 && Player == 4) {
				//ゲーム画面処理
				cout << "CPU(クリティカル防御) vs Player(クリティカル攻撃)" << endl;
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲージリセット
				Player_c_gauge = 0;
				}

				//CPU(ためる) vs Player(クリティカル攻撃)
				else if (CPU == 0 && Player == 4) {
				//ゲーム画面処理
				cout << "CPU(ためる) vs Player(クリティカル攻撃)" << endl;
				//HP処理
				CPU_hp = v_all[0].gethp() - 3;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge() + 1;
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲージリセット
				Player_c_gauge = 0;
				}

				//CPU(しゃがむ) vs Player(クリティカル攻撃)
				else if (CPU == 1 && Player == 4) {
				//HP処理
				CPU_hp = v_all[0].gethp() - 3;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(しゃがむ) vs Player(クリティカル攻撃)" << endl;
				//ゲージリセット
				Player_c_gauge = 0;
				}

				//CPU(ジャンプ) vs Player(クリティカル攻撃)
				else if (CPU == 2 && Player == 4) {
				//HP処理
				CPU_hp = v_all[0].gethp() - 3;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(ジャンプ) vs Player(クリティカル攻撃)" << endl;
				//ゲージリセット
				Player_c_gauge = 0;
				}

				//CPU(カウンター) vs Player(クリティカル攻撃)
				else if (CPU == 3 && Player == 4) {
				//HP処理
				CPU_hp = v_all[0].gethp() - 3;
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(カウンター) vs Player(クリティカル攻撃)" << endl;
				//ゲージリセット
				Player_c_gauge = 0;
				}

				//恐怖値100以上動けない処理
				//CPU(動けない) vs Player(ためる)
				if (CPU == 9 && Player == 0) {
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(動けない) vs Player(ためる)" << endl;
				}

				//CPU(動けない) vs Player(上段攻撃)
				else if (CPU == 9 && Player == 1) {
					//HP処理
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(動けない) vs Player(上段攻撃)" << endl;
				}

				//CPU(動けない) vs Player(下段攻撃)
				else if (CPU == 9 && Player == 2) {
					//HP処理
					CPU_hp = v_all[0].gethp() - 1;
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲーム画面処理
					cout << "CPU(動けない) vs Player(下段攻撃)" << endl;
				}

				//CPU(動けない) vs Player(会話)
				else if (CPU == 9 && Player == 3) {
					//ゲーム画面処理
					cout << "CPU(動けない) vs Player(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					cin >> chat_com;	//脅す[0]or睨む[1]
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 50;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							CPU_fear_value = v_all[0].getfear_value() + 30;
							Player_fear_value = v_all[1].getfear_value();
						}
						else if (suc == 0) {
							CPU_fear_value = v_all[0].getfear_value();
							Player_fear_value = v_all[1].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					//CPU_fear_value = v_all[0].getfear_value();
					//Player_fear_value = v_all[1].getfear_value();
				}

				//CPU(動けない) vs Player(クリティカル攻撃)
				else if (CPU == 9 && Player == 4) {
					//ゲーム画面処理
					cout << "CPU(動けない) vs Player(クリティカル攻撃)" << endl;
					//HP処理
					CPU_hp = v_all[0].gethp() - 3;
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
					//Player_fear_value処理
					CPU_fear_value = v_all[0].getfear_value();
					Player_fear_value = v_all[1].getfear_value();
					//ゲージリセット
					Player_c_gauge = 0;
				}

				//CPU(クリティカル防御) vs Player(動けない)
				else if (CPU == 4 && Player == 9) {
				//ゲーム画面処理
				cout << "CPU(クリティカル防御) vs Player(動けない)" << endl;
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				}

				//CPU(ためる) vs Player(動けない)
				else if (CPU == 0 && Player == 9) {
				//ゲーム画面処理
				cout << "CPU(ためる) vs Player(動けない)" << endl;
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge() + 1;
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				}

				//CPU(しゃがむ) vs Player(動けない)
				else if (CPU == 1 && Player == 9) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(しゃがむ) vs Player(動けない)" << endl;
				}

				//CPU(ジャンプ) vs Player(動けない)
				else if (CPU == 2 && Player == 9) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(ジャンプ) vs Player(動けない)" << endl;
				}

				//CPU(カウンター) vs Player(動けない)
				else if (CPU == 3 && Player == 9) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(カウンター) vs Player(動けない)" << endl;
				}

				//CPU(動けない) vs Player(動けない)
				else if (CPU == 9 && Player == 9) {
				//HP処理
				CPU_hp = v_all[0].gethp();
				Player_hp = v_all[1].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[0].getc_gauge();
				Player_c_gauge = v_all[1].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[0].getfear_value();
				Player_fear_value = v_all[1].getfear_value();
				//ゲーム画面処理
				cout << "CPU(動けない) vs Player(動けない)" << endl;
				}
				
			}

			//else 偶数ターン = 防御側
			else {
				cout << "■防御側■" << endl;

				int Player = command;
				int CPU = 0;

				//CPU防御_行動決定_コマンド選択確率
				if (Player_fear_value >= 100) {
					CPU = 0;
				}
				else {
					//Player(3or2,0or1)
					if ((Player_hp == 3 || Player_hp == 2) && (Player_c_gauge == 0 || Player_c_gauge == 1)) {
						//if CPU(3,0or1), 0[35%], 1[30%], 2[30%], 3[5%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(35, 65, 95);
						}
						//if CPU(2,0or1), 0[38%], 1[30%], 2[30%], 3[2%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(38, 68, 98);
						}
						//if CPU(1,0or1), 0[20%], 1[40%], 2[40%], 3[0%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(20, 60, 100);
						}
						//if CPU(2or3,2), 0[46%], 1[27%], 2[27%], 3[0%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(46, 73, 100);
						}
						//if CPU(1,2), 0[100%], 1[0%], 2[0%], 3[0%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(100, 100, 100);
						}
						//if CPU(2or3,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
						//if CPU(1,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
					}

					//Player(3or2,2)
					else if ((Player_hp == 3 || Player_hp == 2) && Player_c_gauge == 2) {
						//if CPU(3,0or1), 0[35%], 1[32%], 2[32%], 3[1%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(35, 67, 99);
						}
						//if CPU(2,0or1), 0[40%], 1[30%], 2[30%], 3[0%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(40, 70, 100);
						}
						//if CPU(1,0or1), 0[20%], 1[40%], 2[40%], 3[0%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(20, 60, 100);
						}
						//if CPU(2or3,2), 0[46%], 1[27%], 2[27%], 3[0%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(46, 73, 100);
						}
						//if CPU(1,2), 0[100%], 1[0%], 2[0%], 3[0%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(100, 100, 100);
						}
						//if CPU(2or3,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
						//if CPU(1,3), 0[0%], 1[30%], 2[30%], 3[0%], 4[40%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 30, 60, 60);
						}
					}

					//Player(3or2,3)
					else if ((Player_hp == 3 || Player_hp == 2) && Player_c_gauge >= 3) {
						//if CPU(3,0or1), 0[70%], 1[15%], 2[15%], 3[0%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(70, 85, 100);
						}
						//if CPU(2,0or1), 0[70%], 1[15%], 2[15%], 3[0%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(70, 85, 100);
						}
						//if CPU(1,0or1), 0[90%], 1[5%], 2[5%], 3[0%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(90, 95, 100);
						}
						//if CPU(2or3,2), 0[98%], 1[1%], 2[1%], 3[0%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(98, 99, 100);
						}
						//if CPU(1,2), 0[100%], 1[0%], 2[0%], 3[0%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(100, 100, 100);
						}
						//if CPU(2or3,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
						//if CPU(1,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
					}

					//Player(1,0or1or2)
					else if (Player_hp == 1 && (Player_c_gauge == 0 || Player_c_gauge == 1 || Player_c_gauge == 2)) {
						//if CPU(3,0or1), 0[20%], 1[40%], 2[40%], 3[0%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(20, 60, 100);
						}
						//if CPU(2,0or1), 0[34%], 1[33%], 2[33%], 3[0%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(34, 67, 100);
						}
						//if CPU(1,0or1), 0[20%], 1[45%], 2[45%], 3[0%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(10, 55, 100);
						}
						//if CPU(2or3,2), 0[30%], 1[35%], 2[35%], 3[0%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(30, 65, 100);
						}
						//if CPU(1,2), 0[30%], 1[35%], 2[35%], 3[0%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(30, 65, 100);
						}
						//if CPU(2or3,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
						//if CPU(1,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
					}

					//Player(1,3)
					else if (Player_hp == 1 && Player_c_gauge >= 3) {
						//if CPU(3,0or1), 0[20%], 1[40%], 2[40%], 3[0%]
						if (CPU_hp == 3 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(20, 60, 100);
						}
						//if CPU(2,0or1), 0[34%], 1[33%], 2[33%], 3[0%]
						else if (CPU_hp == 2 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(34, 67, 100);
						}
						//if CPU(1,0or1), 0[20%], 1[45%], 2[45%], 3[0%]
						else if (CPU_hp == 1 && (CPU_c_gauge == 0 || CPU_c_gauge == 1)) {
							CPU = CPU_4A_command(10, 55, 100);
						}
						//if CPU(2or3,2), 0[30%], 1[35%], 2[35%], 3[0%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(30, 65, 100);
						}
						//if CPU(1,2), 0[30%], 1[35%], 2[35%], 3[0%]
						else if (CPU_hp == 1 && CPU_c_gauge == 2) {
							CPU = CPU_4A_command(30, 65, 100);
						}
						//if CPU(2or3,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if ((CPU_hp == 2 || CPU_hp == 3) && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
						//if CPU(1,3), 0[0%], 1[25%], 2[25%], 3[0%], 4[50%]
						else if (CPU_hp == 1 && CPU_c_gauge >= 3) {
							CPU = CPU_5A_command(0, 25, 50, 50);
						}
					}

					//予備
					else {
						CPU = CPU_4A_command(40, 60, 80);
					}
				}

				//Player恐怖値100以上で停止
				if (Player_fear_value >= 100) {
					cout << "Playerは動けない" << endl;
					Player = 9;
					Player_fear_value = 0;
					v_all[1].setfear_value(Player_fear_value);
				}
				//CPU恐怖値100以上で停止 & Player行動決定
				else if (CPU_fear_value >= 100) {
					cout << "CPUは動けない" << endl;
					CPU = 9;	//何もできないコマンド
					CPU_fear_value = 0;
					v_all[0].setfear_value(CPU_fear_value);

					//クリティカルゲージ判定
					if (CPU_c_gauge >= 3) {
						//CPUにクリティカル攻撃許可
						cout << "CPUにクリティカル攻撃出現!" << endl;
						cout << "コマンドを選択せよ" << endl;
						cout << ">>ためる[0], しゃがむ[1], ジャンプ[2], カウンター[3], クリティカル防御[4]" << endl;
						cin >> command;
						Player = command;
						if (Player == 0)Player = 0;
						else if (Player == 1)Player = 1;
						else if (Player == 2)Player = 2;
						else if (Player == 3)Player = 3;
						else if (Player == 4)Player = 4;
						else Player = 9;
					}
					else {
						//通常コマンド表示
						cout << "コマンドを選択せよ" << endl;
						cout << ">>ためる[0], しゃがむ[1], ジャンプ[2], カウンター[3]" << endl;
						cin >> command;
						Player = command;
						if (Player == 0)Player = 0;
						else if (Player == 1)Player = 1;
						else if (Player == 2)Player = 2;
						else if (Player == 3)Player = 3;
						else Player = 9;
					}
				}
				//else
				else {
					if (CPU_c_gauge >= 3) {
						//CPUにクリティカル攻撃許可
						cout << "CPUにクリティカル攻撃出現!" << endl;
						cout << "コマンドを選択せよ" << endl;
						cout << ">>ためる[0], しゃがむ[1], ジャンプ[2], カウンター[3], クリティカル防御[4]" << endl;
						cin >> command;
						Player = command;
						if (Player == 0)Player = 0;
						else if (Player == 1)Player = 1;
						else if (Player == 2)Player = 2;
						else if (Player == 3)Player = 3;
						else if (Player == 4)Player = 4;
						else Player = 9;
					}
					else {
						//通常コマンド表示
						cout << "コマンドを選択せよ" << endl;
						cout << ">>ためる[0], しゃがむ[1], ジャンプ[2], カウンター[3]" << endl;
						cin >> command;
						Player = command;
						if (Player == 0)Player = 0;
						else if (Player == 1)Player = 1;
						else if (Player == 2)Player = 2;
						else if (Player == 3)Player = 3;
						else Player = 9;
					}
				}

				//リザルト

				//Player(ためる) vs CPU(ためる)
				if (Player == 0 && CPU == 0) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ためる) vs CPU(ためる)" << endl;
				}

				//Player(ためる) vs CPU(上段攻撃)
				else if (Player == 0 && CPU == 1) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ためる) vs CPU(上段攻撃)" << endl;
				}

				//Player(ためる) vs CPU(下段攻撃)
				else if (Player == 0 && CPU == 2) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ためる) vs CPU(下段攻撃)" << endl;
				}

				//Player(ためる) vs CPU(会話)
				else if (Player == 0 && CPU == 3) {
					//ゲーム画面処理
					cout << "Player(ためる) vs CPU(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					chat_com= Threaten_dist();
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 50;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 30;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 30;
						}
					}
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					//Player_fear_value = v_all[1].getfear_value();
					//CPU_fear_value = v_all[0].getfear_value();
				}

				//Player(しゃがむ) vs CPU(ためる)
				else if (Player == 1 && CPU == 0) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(しゃがむ) vs CPU(ためる)" << endl;
				}

				//Player(しゃがむ) vs CPU(上段攻撃)
				else if (Player == 1 && CPU == 1) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(しゃがむ) vs CPU(上段攻撃)" << endl;
				}

				//Player(しゃがむ) vs CPU(下段攻撃)
				else if (Player == 1 && CPU == 2) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(しゃがむ) vs CPU(下段攻撃)" << endl;
				}

				//Player(しゃがむ) vs CPU(会話)
				else if (Player == 1 && CPU == 3) {
					//ゲーム画面処理
					cout << "Player(しゃがむ) vs CPU(会話)" << endl;
					cout << "会話成功,脅す[0]or睨む[1],の選択" << endl;
					int chat_com;
					chat_com = Threaten_dist();
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 50;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 30;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//Player(ジャンプ) vs CPU(ためる)
				else if (Player == 2 && CPU == 0) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ジャンプ) vs CPU(ためる)" << endl;
				}

				//Player(ジャンプ) vs CPU(上段攻撃)
				else if (Player == 2 && CPU == 1) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ジャンプ) vs CPU(上段攻撃)" << endl;
				}

				//Player(ジャンプ) vs CPU(下段攻撃)
				else if (Player == 2 && CPU == 2) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ジャンプ) vs CPU(下段攻撃)" << endl;
				}

				//Player(ジャンプ) vs CPU(会話)
				else if (Player == 2 && CPU == 3) {
					//ゲーム画面処理
					cout << "Player(ジャンプ) vs CPU(会話)" << endl;
					cout << "会話成功,脅すor睨む,の選択" << endl;
					int chat_com;
					chat_com = Threaten_dist();
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 50;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 30;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//Player(カウンター) vs CPU(ためる)
				else if (Player == 3 && CPU == 0) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(カウンター) vs CPU(ためる)" << endl;
				}

				//Player(カウンター) vs CPU(上段攻撃)
				else if (Player == 3 && CPU == 1) {
					cout << "Player(カウンター) vs CPU(上段攻撃)" << endl;
					suc_c = Counter();
					if (suc_c == 1) {
						Player_hp = v_all[1].gethp();
						CPU_hp = v_all[0].gethp() - 1;
					}
					else if (suc_c == 0) {
						Player_hp = v_all[1].gethp() - 1;
						CPU_hp = v_all[0].gethp();
					}
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
				}

				//Player(カウンター) vs CPU(下段攻撃)
				else if (Player == 3 && CPU == 2) {
					cout << "Player(カウンター) vs CPU(下段攻撃)" << endl;
					suc_c = Counter();
					if (suc_c == 1) {
						Player_hp = v_all[1].gethp();
						CPU_hp = v_all[0].gethp() - 1;
					}
					else if (suc_c == 0) {
						Player_hp = v_all[1].gethp() - 1;
						CPU_hp = v_all[0].gethp();
					}
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
				}

				//Player(カウンター) vs CPU(会話)
				else if (Player == 3 && CPU == 3) {
					//ゲーム画面処理
					cout << "Player(カウンター) vs CPU(会話)" << endl;
					cout << "会話成功,脅すor睨む,の選択" << endl;
					int chat_com;
					chat_com = Threaten_dist();
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 50;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 30;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//クリティカル
				//Player(クリティカル防御) vs CPU(ためる)
				else if (Player == 4 && CPU == 0) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(クリティカル防御) vs CPU(ためる)" << endl;
				}

				//Player(クリティカル防御) vs CPU(上段攻撃)
				else if (Player == 4 && CPU == 1) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(クリティカル防御) vs CPU(上段攻撃)" << endl;
				}

				//Player(クリティカル防御) vs CPU(下段攻撃)
				else if (Player == 4 && CPU == 2) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(クリティカル防御) vs CPU(下段攻撃)" << endl;
				}

				//Player(クリティカル防御) vs CPU(会話)
				else if (Player == 4 && CPU == 3) {
					//ゲーム画面処理
					cout << "Player(クリティカル防御) vs CPU(会話)" << endl;
					cout << "会話成功,脅すor睨む,の選択" << endl;
					int chat_com;
					chat_com = Threaten_dist();
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 50;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 30;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 30;
						}
					}
					//HP処理
					CPU_hp = v_all[0].gethp();
					Player_hp = v_all[1].gethp();
					//c_gauge処理
					CPU_c_gauge = v_all[0].getc_gauge();
					Player_c_gauge = v_all[1].getc_gauge();
				}

				//Player(クリティカル防御) vs CPU(クリティカル攻撃)
				else if (Player == 4 && CPU == 4) {
					//ゲーム画面処理
					cout << "Player(クリティカル防御) vs CPU(クリティカル攻撃)" << endl;
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲージリセット
					CPU_c_gauge = 0;
				}

				//Player(ためる) vs CPU(クリティカル攻撃)
				else if (Player == 0 && CPU == 4) {
					//ゲーム画面処理
					cout << "Player(ためる) vs CPU(クリティカル攻撃)" << endl;
					//HP処理
					Player_hp = v_all[1].gethp() - 3;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲージリセット
					CPU_c_gauge = 0;
				}

				//Player(しゃがむ) vs CPU(クリティカル攻撃)
				else if (Player == 1 && CPU == 4) {
					//HP処理
					Player_hp = v_all[1].gethp() - 3;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲージリセット
					CPU_c_gauge = 0;
					//ゲーム画面処理
					cout << "Player(しゃがむ) vs CPU(クリティカル攻撃)" << endl;
				}

				//Player(ジャンプ) vs CPU(クリティカル攻撃)
				else if (Player == 2 && CPU == 4) {
					//HP処理
					Player_hp = v_all[1].gethp() - 3;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲージリセット
					CPU_c_gauge = 0;
					//ゲーム画面処理
					cout << "Player(ジャンプ) vs CPU(クリティカル攻撃)" << endl;
				}

				//Player(カウンター) vs CPU(クリティカル攻撃)
				else if (Player == 3 && CPU == 4) {
					//HP処理
					Player_hp = v_all[1].gethp() - 3;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲージリセット
					CPU_c_gauge = 0;
					//ゲーム画面処理
					cout << "Player(カウンター) vs CPU(クリティカル攻撃)" << endl;
				}

				//恐怖値100以上動けない処理
				//Player(動けない) vs CPU(ためる)
				if (Player == 9 && CPU == 0) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge() + 1;
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(動けない) vs CPU(ためる)" << endl;
				}

				//Player(動けない) vs CPU(上段攻撃)
				else if (Player == 9 && CPU == 1) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(動けない) vs CPU(上段攻撃)" << endl;
				}

				//Player(動けない) vs CPU(下段攻撃)
				else if (Player == 9 && CPU == 2) {
					//HP処理
					Player_hp = v_all[1].gethp() - 1;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(動けない) vs CPU(下段攻撃)" << endl;
				}

				//Player(動けない) vs CPU(会話)
				else if (Player == 9 && CPU == 3) {
					//ゲーム画面処理
					cout << "Player(動けない) vs CPU(会話)" << endl;
					cout << "会話成功,脅すor睨む,の選択" << endl;
					int chat_com;
					chat_com = Threaten_dist();
					if (chat_com == 0) {
						cout << "「脅す」を選択" << endl;
						suc = Threaten_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 50;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 50;
						}
					}
					else {
						cout << "「睨む」を選択" << endl;
						suc = Gaze_dist();
						if (suc == 1) {
							Player_fear_value = v_all[1].getfear_value() + 30;
							CPU_fear_value = v_all[0].getfear_value();
						}
						else if (suc == 0) {
							Player_fear_value = v_all[1].getfear_value();
							CPU_fear_value = v_all[0].getfear_value() + 30;
						}
					}
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					//Player_fear_value = v_all[1].getfear_value();
					//CPU_fear_value = v_all[0].getfear_value();
				}

				//Player(動けない) vs CPU(クリティカル攻撃)
				else if (Player == 9 && CPU == 4) {
					//ゲーム画面処理
					cout << "Player(動けない) vs CPU(クリティカル攻撃)" << endl;
					//HP処理
					Player_hp = v_all[1].gethp() - 3;
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲージリセット
					CPU_c_gauge = 0;
				}

				//Player(クリティカル防御) vs CPU(動けない)
				else if (Player == 4 && CPU == 9) {
					//ゲーム画面処理
					cout << "Player(クリティカル防御) vs CPU(動けない)" << endl;
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
				}

				//Player(ためる) vs CPU(動けない)
				else if (Player == 0 && CPU == 9) {
					//ゲーム画面処理
					cout << "Player(ためる) vs CPU(動けない)" << endl;
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge() + 1;
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
				}

				//Player(しゃがむ) vs CPU(動けない)
				else if (Player == 1 && CPU == 9) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(しゃがむ) vs CPU(動けない)" << endl;
				}

				//Player(ジャンプ) vs CPU(動けない)
				else if (Player == 2 && CPU == 9) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(ジャンプ) vs CPU(動けない)" << endl;
				}

				//Player(カウンター) vs CPU(動けない)
				else if (Player == 3 && CPU == 9) {
					//HP処理
					Player_hp = v_all[1].gethp();
					CPU_hp = v_all[0].gethp();
					//c_gauge処理
					Player_c_gauge = v_all[1].getc_gauge();
					CPU_c_gauge = v_all[0].getc_gauge();
					//CPU_fear_value処理
					Player_fear_value = v_all[1].getfear_value();
					CPU_fear_value = v_all[0].getfear_value();
					//ゲーム画面処理
					cout << "Player(カウンター) vs CPU(動けない)" << endl;
				}

				//CPU(動けない) vs Player(動けない)
				else if (CPU == 9 && Player == 9) {
				//HP処理
				CPU_hp = v_all[1].gethp();
				Player_hp = v_all[0].gethp();
				//c_gauge処理
				CPU_c_gauge = v_all[1].getc_gauge();
				Player_c_gauge = v_all[0].getc_gauge();
				//Player_fear_value処理
				CPU_fear_value = v_all[1].getfear_value();
				Player_fear_value = v_all[0].getfear_value();
				//ゲーム画面処理
				cout << "CPU(動けない) vs Player(動けない)" << endl;
				}
				
			}

			//状態更新
			v_all[0].sethp(CPU_hp);
			v_all[1].sethp(Player_hp);
			v_all[0].setc_gauge(CPU_c_gauge);
			v_all[1].setc_gauge(Player_c_gauge);
			v_all[0].setfear_value(CPU_fear_value);
			v_all[1].setfear_value(Player_fear_value);

			
			cout << "Player" << endl;
			cout << ">PlayerHP : " << Player_hp << endl;
			cout << ">Playerクリティカルゲージ : " << Player_c_gauge << endl;
			cout << ">Player恐怖値 : " << Player_fear_value << endl;
			cout << endl;
			cout << "CPU" << endl;
			cout << ">CPUHP : " << CPU_hp << endl;
			cout << ">CPUクリティカルゲージ : " << CPU_c_gauge << endl;
			cout << ">CPU恐怖値 : " << CPU_fear_value << endl;
			cout << endl;

		}
	}

}