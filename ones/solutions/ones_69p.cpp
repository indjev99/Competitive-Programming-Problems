#include<bits/stdc++.h>
#include "ones.h"
using namespace std ;
mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

int flip_bits ( const vector < bool > &flips ) ;

pair < int , int > find_longest_subarray_of_ones ( int n ) {
    vector < bool > arg ( n , 0 ) ;
    vector < bool > fl ( n , 1 ) ;
    for ( int i = 0 ; i < n ; ++ i ) {
        arg[ i ] = rng ( ) % 2 ;
    }
    int init = flip_bits ( arg ) ;
    for ( int i = 0 ; i < n ; ++ i ) {
        arg[ i ] = 0 ;
    }    
    if ( init == n ) { return { 0 , n - 1 } ; }
    int sr = flip_bits ( fl ) ;
    if ( sr == n ) { return { 0 , n - 1 } ; }
    flip_bits ( fl ) ;
    auto set_pref = [ & ] ( int len ) {
        for ( int i = 0 ; i < len ; ++ i ) {
            arg[ i ] = 1 ;
        }
    };
    auto rem_pref = [ & ] ( int len ) {
        for ( int i = 0 ; i < len ; ++ i ) {
            arg[ i ] = 0 ;
        }
    };
    auto get_pref_val = [ & ] ( int len , bool revert ) {
        set_pref ( len ) ;
        int ret = flip_bits ( arg ) ;
        if ( revert == true ) { flip_bits ( arg ) ; }
        rem_pref ( len ) ;
        return ret ;
    };
    while ( init == sr ) {
        for ( int i = 0 ; i < n ; ++ i ) {
            arg[ i ] = rng ( ) % 2 ;
        }
        init = flip_bits ( arg ) ;
        for ( int i = 0 ; i < n ; ++ i ) {
            arg[ i ] = 0 ;
        }
        sr = flip_bits ( fl ) ;
        flip_bits ( fl ) ;
    }
    if ( init > sr ) {
        flip_bits ( fl ) ;
        swap ( init , sr ) ;
    }
    int prv_q = -1 ;
    int l , r , mid ;
    l = 1 , r = n ;
    int lst_ans = init ;
    while ( l < r ) {
        mid = ( l + r ) / 2 ;
        int hh = get_pref_val ( mid , true ) ;
        if ( hh <= init ) { l = mid + 1 ; }
        else { r = mid ; }
    }
    // l - 1 guaranteed in max range
    lst_ans = get_pref_val ( l , false ) ;
    // cout << "inside --> " << l - 1 << ", init = " << init << "  len = " << lst_ans << "\n" ;
    
    for ( int j = 0 ; j < n ; ++ j ) { arg[ j ] = 0 ; }
    
    for ( int i = l - 2 ; i >= 0 ; -- i ) {
        // check if this starts at i + 1 
        arg[ i ] = 1 ;
        int ret = flip_bits ( arg ) ;
        flip_bits ( arg ) ;
        arg[ i ] = 0 ;
        if ( ret > lst_ans ) {
            return { i + 1 , i + lst_ans } ;
        }
    }
    return { 0 , lst_ans - 1 } ;
}


