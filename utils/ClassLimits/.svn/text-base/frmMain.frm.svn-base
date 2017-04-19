VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Wizard Wars: ClassLimits Tool"
   ClientHeight    =   3015
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4110
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3015
   ScaleWidth      =   4110
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkClass 
      Caption         =   "ArchMage Only"
      Height          =   375
      Index           =   10
      Left            =   120
      TabIndex        =   11
      Top             =   2520
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Random PC"
      Height          =   375
      Index           =   9
      Left            =   2040
      TabIndex        =   10
      Top             =   2040
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Nature Wizard"
      Height          =   375
      Index           =   8
      Left            =   2040
      TabIndex        =   9
      Top             =   1680
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Glacist"
      Height          =   375
      Index           =   7
      Left            =   2040
      TabIndex        =   8
      Top             =   1320
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Pyromancer"
      Height          =   375
      Index           =   6
      Left            =   2040
      TabIndex        =   7
      Top             =   960
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Geomancer"
      Height          =   375
      Index           =   5
      Left            =   2040
      TabIndex        =   6
      Top             =   600
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Healer"
      Height          =   375
      Index           =   4
      Left            =   120
      TabIndex        =   5
      Top             =   2040
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Necromancer"
      Height          =   375
      Index           =   3
      Left            =   120
      TabIndex        =   4
      Top             =   1680
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Dracomancer"
      Height          =   375
      Index           =   2
      Left            =   120
      TabIndex        =   3
      Top             =   1320
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Tempest"
      Height          =   375
      Index           =   1
      Left            =   120
      TabIndex        =   2
      Top             =   960
      Width           =   1815
   End
   Begin VB.CheckBox chkClass 
      Caption         =   "No Aeromancer"
      Height          =   375
      Index           =   0
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   1815
   End
   Begin VB.TextBox txtBits 
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Text            =   "0"
      Top             =   120
      Width           =   3855
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const BIT_WIND = 1
Const BIT_LITN = 2
Const BIT_DRGN = 64
Const BIT_NCRO = 8
Const BIT_LIFE = 16
Const BIT_ERTH = 32
Const BIT_FIRE = 4
Const BIT_RAND = 128
Const BIT_IICE = 256
Const BIT_NTRE = 512
Const BIT_MAGE = -1

Private Sub chkClass_Click(Index As Integer)

    Dim iBit As Integer

    iBit = 0

    If chkClass(0).Value Then iBit = iBit Or BIT_WIND

    If chkClass(1).Value Then iBit = iBit Or BIT_LITN

    If chkClass(2).Value Then iBit = iBit Or BIT_DRGN

    If chkClass(3).Value Then iBit = iBit Or BIT_NCRO

    If chkClass(4).Value Then iBit = iBit Or BIT_LIFE

    If chkClass(5).Value Then iBit = iBit Or BIT_ERTH

    If chkClass(6).Value Then iBit = iBit Or BIT_FIRE

    If chkClass(7).Value Then iBit = iBit Or BIT_IICE

    If chkClass(8).Value Then iBit = iBit Or BIT_NTRE

    If chkClass(9).Value Then iBit = iBit Or BIT_RAND

    If chkClass(10).Value Then iBit = -1
        
    txtBits.Text = iBit

End Sub

