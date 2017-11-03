//
//  Mathematics.h
//  Cocos2dxPractice
//
//  Created by Tomoya Fujii on 2017/06/25.
//
//

#ifndef Mathematics_h
#define Mathematics_h
#include <math.h>

//========================================================
// 2次元ベクトルクラス
//========================================================

class Vector2 {
public:
    //コンストラクタ
    Vector2(){}
    Vector2(float x, float y): m_x( x ) , m_y( y ) {}
    
public:
    float m_x;
    float m_y;
    
    /**
     * @desc    設定用関数
     * @param   x 座標
     * @param   y 座標
     */
    void set(float x,float y){
        this->m_x = x;
        this->m_y = y;
    }
    
    void set( Vector2 vec ){
        this->m_x = vec.m_x ;
        this->m_y = vec.m_y ;
    }
    
    /**
     * @desc    ベクトルの大きさを求める
     * @return  ベクトルの大きさ
     */
    float norm(){
        return sqrtf(this->m_x * this->m_x + this->m_y * this->m_y);
    }
    
    /**
     * @desc    ２点間の距離を求める
     * @param   dest    目的の座標
     * @return  目的までの距離
     */
    float distance(Vector2* dest){
        float distX = dest->m_x - this->m_x ;
        float distY = dest->m_y - this->m_y ;
        return sqrt( distX * distX + distY * distY ) ;
    }
    
    /**
     * @desc    内積
     * @param   vec 対象となるもう一つのベクトル
     * @return  内積の値
     */
    float dot(Vector2* vec){
        return this->m_x * vec->m_y + this->m_y * vec->m_x;
    }
    
    /**
     * @desc    外積
     * @param   vec 対象となるもう一つのベクトル
     * @return  外積の値
     */
    float cross(Vector2* vec){
        return this->m_x * vec->m_y - this->m_y * vec->m_x;
    }
    
    /**
     * @desc    単位ベクトル取得
     */
    Vector2 getUnitVector(){
        float norm = this->norm();
        return Vector2( this->m_x / norm, this->m_y / norm );
    }
    
/////////////////////////////////////////////
//Vector2用 演算子のオーバーロード
/////////////////////////////////////////////
    
    // + 演算子のオーバーロード
    Vector2 operator +(Vector2 vec)
    {
        return Vector2( this->m_x + vec.m_x,this->m_y + vec.m_y);
    }
    
    // - 演算子のオーバーロード
    Vector2 operator -(Vector2 vec)
    {
        return Vector2( this->m_x - vec.m_x,this->m_y - vec.m_y);
    }
    
    // * 演算子のオーバーロード
    Vector2 operator *(float param)
    {
        return Vector2( this->m_x * param ,this->m_y * param);
    }
    
    // / 演算子のオーバーロード
    Vector2 operator /(float param)
    {
        return Vector2( this->m_x / param ,this->m_y / param);
    }
    
    // = 演算子のオーバーロード
    Vector2 operator =(Vector2 vec)
    {
        return Vector2( this->m_x = vec.m_x,this->m_y = vec.m_y);
    }
    
    // += 演算子のオーバーロード
    Vector2 operator +=(Vector2 vec)
    {
        return Vector2( this->m_x += vec.m_x,this->m_y += vec.m_y);
    }
    
    // -= 演算子のオーバーロード
    Vector2 operator -=(Vector2 vec)
    {
        return Vector2( this->m_x -= vec.m_x,this->m_y -= vec.m_y);
    }
    
    // *= 演算子のオーバーロード
    Vector2 operator *=(float param)
    {
        return Vector2( this->m_x *= param ,this->m_y *= param);
    }
    
    // /= 演算子のオーバーロード
    Vector2 operator /=(float param)
    {
        return Vector2( this->m_x /= param ,this->m_y /= param);
    }
    
    //== 演算子のオーバーロード
    bool operator ==( Vector2 vec ){
        return this->m_x == vec.m_x && this->m_y == vec.m_y ;
    }
    
    //!= 演算子のオーバーロード
    bool operator != ( Vector2 vec ){
        return !(this->m_x == vec.m_x && this->m_y == vec.m_y) ;
    }
/////////////////////////////////////////////
//演算子のオーバーロード終わり
/////////////////////////////////////////////

};

using Point = Vector2 ;

class CLine {
public:
    //コンストラクタ
	CLine(): m_setStartPoint(false),m_setEndPoint(false),m_startPoint(Point(0,0)),m_endPoint(Point(0,0)),m_vector(Vector2(0, 0)){}
    CLine( Point startPoint, Point endPoint ) : m_startPoint(startPoint),m_endPoint(endPoint){
        this->m_vector = endPoint - startPoint ;
    }
private:
    //始点設定済みフラグ
    bool m_setStartPoint = false ;
    //終点設定済みフラグ
    bool m_setEndPoint = false ;
    
public:
    //始点
    Point m_startPoint ;
    //終点
    Point m_endPoint ;
    //ベクトル
    Vector2 m_vector ;
    
public:
    //始点設定
    void setStartPoint( Point point ){
        this->m_startPoint = point ;
        m_setStartPoint = true ;
        //終点が設定されていたら
        if( m_setEndPoint ){
			this->m_vector = this->m_endPoint - this->m_startPoint ;
        }
    }
    //終点設定
    void setEndPoint( Point point ){
        this->m_endPoint = point ;
        this->m_setEndPoint = true ;
        //始点が設定されていたら
        if( m_setStartPoint ){
			this->m_vector = this->m_endPoint - this->m_startPoint ;
        }
    }
	//ベクトル設定
	void setVector( Vector2 vec ){
		this->m_vector = vec ;
		if( this->m_setStartPoint == true ){
			this->m_endPoint = this->m_startPoint + this->m_vector ;
		}
	}
    //始点終点設定
    void set( Point startPoint, Point endPoint ){
        this->m_startPoint = startPoint ;
        this->m_endPoint = endPoint ;
		this->m_vector = endPoint - startPoint ;
	}
};
#endif /* Mathematics_h */
